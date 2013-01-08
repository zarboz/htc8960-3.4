/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "../../../../drivers/video/msm/msm_fb.h"
#include "../../../../drivers/video/msm/mipi_dsi.h"
#include "../../../../drivers/video/msm/mdp4.h"
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <mach/panel_id.h>
#include <mach/debug_display.h>
#include <mach/msm_memtypes.h>

#include "../devices.h"
#include "../board-jet.h"

#define MSM_FB_SIZE roundup(MSM_FB_PRIM_BUF_SIZE + MSM_FB_EXT_BUF_SIZE, 4096)

#ifdef CONFIG_FB_MSM_OVERLAY0_WRITEBACK
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((960 * 544 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY0_WRITEBACK */

#ifdef CONFIG_FB_MSM_OVERLAY1_WRITEBACK
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE roundup((1920 * 1088 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY1_WRITEBACK */

/* Select panel operate mode : CMD, VIDEO or SWITCH mode */

int jet_panel_first_init = 1;
static bool dsi_power_on;

static int mipi_dsi_panel_power(int on)
{
	static struct regulator *v_lcm, *v_lcmio, *v_dsivdd;
	static bool bPanelPowerOn = false;
	int rc;

	char *lcm_str = "8921_l11";
	char *lcmio_str = "8921_lvs5";
	char *dsivdd_str = "8921_l2";

	PR_DISP_INFO("%s: state : %d\n", __func__, on);

	if (!dsi_power_on) {

		v_lcm = regulator_get(&msm_mipi_dsi1_device.dev,
				lcm_str);
		if (IS_ERR(v_lcm)) {
			PR_DISP_ERR("could not get %s, rc = %ld\n",
				lcm_str, PTR_ERR(v_lcm));
			return -ENODEV;
		}

		v_lcmio = regulator_get(&msm_mipi_dsi1_device.dev,
				lcmio_str);
		if (IS_ERR(v_lcmio)) {
			PR_DISP_ERR("could not get %s, rc = %ld\n",
				lcmio_str, PTR_ERR(v_lcmio));
			return -ENODEV;
		}

		v_dsivdd = regulator_get(&msm_mipi_dsi1_device.dev,
				dsivdd_str);
		if (IS_ERR(v_dsivdd)) {
			PR_DISP_ERR("could not get %s, rc = %ld\n",
				dsivdd_str, PTR_ERR(v_dsivdd));
			return -ENODEV;
		}
		if (system_rev >= 2) /* XC */
			rc = regulator_set_voltage(v_lcm, 3000000, 3000000);
		else
			rc = regulator_set_voltage(v_lcm, 3200000, 3200000);
		if (rc) {
			PR_DISP_ERR("%s#%d: set_voltage %s failed, rc=%d\n", __func__, __LINE__, lcm_str, rc);
			return -EINVAL;
		}

		rc = regulator_set_voltage(v_dsivdd, 1200000, 1200000);
		if (rc) {
			PR_DISP_ERR("%s#%d: set_voltage %s failed, rc=%d\n", __func__, __LINE__, dsivdd_str, rc);
			return -EINVAL;
		}

		rc = gpio_request(JET_GPIO_LCD_RSTz, "LCM_RST_N");
		if (rc) {
			PR_DISP_ERR("%s:LCM gpio %d request failed, rc=%d\n", __func__,  JET_GPIO_LCD_RSTz, rc);
			return -EINVAL;
		}

		dsi_power_on = true;
	}

	if (on) {
		PR_DISP_INFO("%s: on\n", __func__);
		rc = regulator_set_optimum_mode(v_lcm, 100000);
		if (rc < 0) {
			PR_DISP_ERR("set_optimum_mode %s failed, rc=%d\n", lcm_str, rc);
			return -EINVAL;
		}

		rc = regulator_set_optimum_mode(v_dsivdd, 100000);
		if (rc < 0) {
			PR_DISP_ERR("set_optimum_mode %s failed, rc=%d\n", dsivdd_str, rc);
			return -EINVAL;
		}

		rc = regulator_enable(v_lcm);
		if (rc) {
			PR_DISP_ERR("enable regulator %s failed, rc=%d\n", lcm_str, rc);
			return -ENODEV;
		}

		rc = regulator_enable(v_dsivdd);
		if (rc) {
			PR_DISP_ERR("enable regulator %s failed, rc=%d\n", dsivdd_str, rc);
			return -ENODEV;
		}
		rc = regulator_enable(v_lcmio);
		if (rc) {
			PR_DISP_ERR("enable regulator %s failed, rc=%d\n", lcmio_str, rc);
			return -ENODEV;
		}

		jet_lcd_id_power(PM_GPIO_PULL_NO);

		if (!jet_panel_first_init) {
			hr_msleep(20);
			gpio_set_value(JET_GPIO_LCD_RSTz, 1);
			hr_msleep(1);
		}

		bPanelPowerOn = true;

	} else {
		PR_DISP_INFO("%s: off\n", __func__);
		if (!bPanelPowerOn) return 0;
		jet_lcd_id_power(PM_GPIO_PULL_DN);

		gpio_set_value(JET_GPIO_LCD_RSTz, 0);
		hr_msleep(10);

		if (regulator_disable(v_lcmio)) {
			PR_DISP_ERR("%s: Unable to enable the regulator: %s\n", __func__, lcmio_str);
			return -EINVAL;
		}

		if (regulator_disable(v_dsivdd)) {
			PR_DISP_ERR("%s: Unable to enable the regulator: %s\n", __func__, dsivdd_str);
			return -EINVAL;
		}

		if (regulator_disable(v_lcm)) {
			PR_DISP_ERR("%s: Unable to enable the regulator: %s\n", __func__, lcm_str);
			return -EINVAL;
		}

		rc = regulator_set_optimum_mode(v_dsivdd, 100);
		if (rc < 0) {
			PR_DISP_ERR("%s: Unable to enable the regulator: %s\n", __func__, dsivdd_str);
			return -EINVAL;
		}

		bPanelPowerOn = false;
	}
	return 0;
}

static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.vsync_gpio = JET_GPIO_LCD_TE,
	.dsi_power_save = mipi_dsi_panel_power,
};

static struct platform_device mipi_dsi_jet_panel_device = {
  .name = "mipi_jet",
  .id = 0,
};

#ifdef CONFIG_MSM_BUS_SCALING
static struct msm_bus_vectors dtv_bus_init_vectors[] = {
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 0,
    .ib = 0,
  },
};
static struct msm_bus_vectors dtv_bus_def_vectors[] = {
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 566092800 * 2,
    .ib = 707616000 * 2,
  },
};
static struct msm_bus_paths dtv_bus_scale_usecases[] = {
  {
    ARRAY_SIZE(dtv_bus_init_vectors),
    dtv_bus_init_vectors,
  },
  {
    ARRAY_SIZE(dtv_bus_def_vectors),
    dtv_bus_def_vectors,
  },
};
static struct msm_bus_scale_pdata dtv_bus_scale_pdata = {
  dtv_bus_scale_usecases,
  ARRAY_SIZE(dtv_bus_scale_usecases),
  .name = "dtv",
};

static struct lcdc_platform_data dtv_pdata = {
  .bus_scale_table = &dtv_bus_scale_pdata,
};

static struct msm_bus_vectors mdp_init_vectors[] = {
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 0,
    .ib = 0,
  },
};

static struct msm_bus_vectors mdp_ui_vectors[] = {
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 216000000 * 2,
    .ib = 270000000 * 2,
  },
};

static struct msm_bus_vectors mdp_vga_vectors[] = {
  /* VGA and less video */
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 216000000 * 2,
    .ib = 270000000 * 2,
  },
};

static struct msm_bus_vectors mdp_720p_vectors[] = {
  /* 720p and less video */
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 230400000 * 2,
    .ib = 288000000 * 2,
  },
};

static struct msm_bus_vectors mdp_1080p_vectors[] = {
  /* 1080p and less video */
  {
    .src = MSM_BUS_MASTER_MDP_PORT0,
    .dst = MSM_BUS_SLAVE_EBI_CH0,
    .ab = 334080000 * 2,
    .ib = 417600000 * 2,
  },
};

static struct msm_bus_paths mdp_bus_scale_usecases[] = {
  {
    ARRAY_SIZE(mdp_init_vectors),
    mdp_init_vectors,
  },
  {
    ARRAY_SIZE(mdp_ui_vectors),
    mdp_ui_vectors,
  },
  {
    ARRAY_SIZE(mdp_ui_vectors),
    mdp_ui_vectors,
  },
  {
    ARRAY_SIZE(mdp_vga_vectors),
    mdp_vga_vectors,
  },
  {
    ARRAY_SIZE(mdp_720p_vectors),
    mdp_720p_vectors,
  },
  {
    ARRAY_SIZE(mdp_1080p_vectors),
    mdp_1080p_vectors,
  },
};

static struct msm_bus_scale_pdata mdp_bus_scale_pdata = {
  mdp_bus_scale_usecases,
  ARRAY_SIZE(mdp_bus_scale_usecases),
  .name = "mdp",
};

#endif

int mdp_core_clk_rate_table[] = {
  85330000,
  85330000,
  160000000,
  200000000,
};

static struct msm_panel_common_pdata mdp_pdata = {
  .gpio = JET_GPIO_LCD_TE,
#ifdef CONFIG_MSM_BUS_SCALING
  .mdp_bus_scale_table = &mdp_bus_scale_pdata,
#endif
  .mdp_rev = MDP_REV_42,
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
  .mem_hid = BIT(ION_CP_MM_HEAP_ID),
#else
  .mem_hid = MEMTYPE_EBI1,
#endif
  .mdp_max_clk = 200000000,
};

void __init msm8960_mdp_writeback(struct memtype_reserve* reserve_table)
{
  mdp_pdata.ov0_wb_size = MSM_FB_OVERLAY0_WRITEBACK_SIZE;
  mdp_pdata.ov1_wb_size = MSM_FB_OVERLAY1_WRITEBACK_SIZE;
#if defined(CONFIG_ANDROID_PMEM) && !defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
  reserve_table[mdp_pdata.mem_hid].size +=
    mdp_pdata.ov0_wb_size;
  reserve_table[mdp_pdata.mem_hid].size +=
    mdp_pdata.ov1_wb_size;
#endif
}

void __init jet_init_fb(void)
{
  platform_device_register(&mipi_dsi_jet_panel_device);
  msm_fb_register_device("mdp", &mdp_pdata);
  msm_fb_register_device("mipi_dsi", &mipi_dsi_pdata);
  msm_fb_register_device("dtv", &dtv_pdata);
}

static int __init jet_init_panel(void)
{
  printk(KERN_INFO "%s: panel type = 0x%x, hboot type = 0x%x, board_mfg_mode = 0x%x\n", 
         __func__, panel_type, board_build_flag(), board_mfg_mode());
  
  if (panel_type == PANEL_ID_NONE) 
    {
      if (board_mfg_mode() == 8 || board_mfg_mode() == 4) 
        {
          PR_DISP_INFO("%s: enter mfgkernel/powettest \n", __func__);
          return 0;
        } 
      else if (board_build_flag() == SHIP_BUILD) 
        {
          /* For shipping produce, the main source panel can cover the most device */
          PR_DISP_INFO("%s: Use PANEL_ID_JET_SONY_NT_C2 be default panel!\n", __func__);
          panel_type = PANEL_ID_JET_SONY_NT_C2;
        } 
      else
        panic("[DISP] PANEL_ID_NONE! panic");
    }
  return 0;
}

module_init(jet_init_panel);
