/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#include <asm/mach-types.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <mach/board.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include <asm/setup.h>

#include "devices.h"
#include "board-elite.h"

#include <linux/spi/spi.h>

#include "board-mahimahi-flashlight.h"
#ifdef CONFIG_MSM_CAMERA_FLASH
#include <linux/htc_flashlight.h>
#endif

#ifdef CONFIG_I2C

#define MSM_8960_GSBI4_QUP_I2C_BUS_ID 4

#endif
static int camera_sensor_power_enable(char *power, unsigned volt, struct regulator **sensor_power);
static int camera_sensor_power_disable(struct regulator *sensor_power);

#ifdef CONFIG_MSM_CAMERA
#ifdef CONFIG_MSM_CAMERA_FLASH
#define LED_ON				1
#define LED_OFF				0

#define HW_VER_ID_VIRT          (MSM_TLMM_BASE + 0x00002054)

static int elite_flashlight_control(int mode)
{
#ifdef CONFIG_FLASHLIGHT_TPS61310
	return tps61310_flashlight_control(mode);
#else
	return 0;
#endif
}

static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER,
	.camera_flash = elite_flashlight_control,
};
#endif

/*
8921_lvs6 == V_CAMIO_1V8
GPIO#4 == CAM2_MCLK
GPIO#93 == V_CAM_D1V8
8921_l8 == V_CAM_A2V8
8921_l9 == V_CAM_VCM2V8
*/
#ifdef CONFIG_RAWCHIP
static int elite_use_ext_1v2(void)
{
	if (system_rev >= 1) /* for XB */
		return 1;
	else /* for XA */
		return 0;
}

static struct regulator *reg_8921_l2;
static struct regulator *reg_8921_l8;
static struct regulator *reg_8921_l9;
static struct regulator *reg_8921_lvs6;

#define ELITE_V_RAW_1V2_EN PM8921_GPIO_PM_TO_SYS(ELITE_PMGPIO_V_RAW_1V2_EN)
static int elite_rawchip_vreg_on(void)
{
	int rc;
	pr_info("[CAM] %s\n", __func__);

	/* PM8921_GPIO_PM_TO_SYS(ELITE_GPIO_V_RAW_1V8_EN) 1800000 */
	rc = gpio_request(ELITE_GPIO_V_RAW_1V8_EN, "V_RAW_1V8_EN");
	if (rc) {
		pr_err("[CAM] rawchip on\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_RAW_1V8_EN, rc);
		goto enable_1v8_fail;
	}
	gpio_direction_output(ELITE_GPIO_V_RAW_1V8_EN, 1);
	gpio_free(ELITE_GPIO_V_RAW_1V8_EN);

	mdelay(5);

	if (system_rev >= 0 && system_rev <= 3) { /* for XA~XD */
	/* PM8921_GPIO_PM_TO_SYS(ELITE_V_RAW_1V2_EN) 1200000 */
	rc = gpio_request(ELITE_V_RAW_1V2_EN, "V_RAW_1V2_EN");
	if (rc) {
		pr_err("[CAM] rawchip on\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_V_RAW_1V2_EN, rc);
		goto enable_1v2_fail;
	}
	gpio_direction_output(ELITE_V_RAW_1V2_EN, 1);
	gpio_free(ELITE_V_RAW_1V2_EN);
	}

	if (system_rev >= 1) { /* for XB */
		if (elite_use_ext_1v2()) { /* use external 1v2 for HW workaround */
			mdelay(1);

			rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "rawchip");
			pr_info("[CAM] rawchip external 1v2 gpio_request,%d\n", ELITE_GPIO_V_CAM_D1V2_EN);
			if (rc < 0) {
				pr_err("GPIO(%d) request failed", ELITE_GPIO_V_CAM_D1V2_EN);
				goto enable_ext_1v2_fail;
			}
			gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 1);
			gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
		}
	}

	return rc;

enable_ext_1v2_fail:
	if (system_rev >= 0 && system_rev <= 3) { /* for XA~XD */
	rc = gpio_request(ELITE_V_RAW_1V2_EN, "V_RAW_1V2_EN");
	if (rc)
		pr_err("[CAM] rawchip on\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_V_RAW_1V2_EN, rc);
	gpio_direction_output(ELITE_V_RAW_1V2_EN, 0);
	gpio_free(ELITE_V_RAW_1V2_EN);
	}
enable_1v2_fail:
	rc = gpio_request(ELITE_GPIO_V_RAW_1V8_EN, "V_RAW_1V8_EN");
	if (rc)
		pr_err("[CAM] rawchip on\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_RAW_1V8_EN, rc);
	gpio_direction_output(ELITE_GPIO_V_RAW_1V8_EN, 0);
	gpio_free(ELITE_GPIO_V_RAW_1V8_EN);
enable_1v8_fail:
	return rc;
}

static int elite_rawchip_vreg_off(void)
{
	int rc = 0;

	pr_info("[CAM] %s\n", __func__);

	if (system_rev >= 1) { /* for XB */
		if (elite_use_ext_1v2()) { /* use external 1v2 for HW workaround */
			rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "rawchip");
			if (rc)
				pr_err("[CAM] rawchip off(\
					\"gpio %d\", 1.2V) FAILED %d\n",
					ELITE_GPIO_V_CAM_D1V2_EN, rc);
			gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 0);
			gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);

			mdelay(1);
		}
	}

	if (system_rev >= 0 && system_rev <= 3) { /* for XA~XD */
	rc = gpio_request(ELITE_V_RAW_1V2_EN, "V_RAW_1V2_EN");
	if (rc)
		pr_err("[CAM] rawchip off(\
			\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_V_RAW_1V2_EN, rc);
	gpio_direction_output(ELITE_V_RAW_1V2_EN, 0);
	gpio_free(ELITE_V_RAW_1V2_EN);
	}

	mdelay(5);

	rc = gpio_request(ELITE_GPIO_V_RAW_1V8_EN, "V_RAW_1V8_EN");
	if (rc)
		pr_err("[CAM] rawchip off\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_RAW_1V8_EN, rc);
	gpio_direction_output(ELITE_GPIO_V_RAW_1V8_EN, 0);
	gpio_free(ELITE_GPIO_V_RAW_1V8_EN);

	return rc;
}

static struct msm_camera_rawchip_info msm_rawchip_board_info = {
	.rawchip_reset	= ELITE_GPIO_RAW_RSTN,
	.rawchip_intr0	= MSM_GPIO_TO_INT(ELITE_GPIO_RAW_INTR0),
        .rawchip_intr1	= MSM_GPIO_TO_INT(ELITE_GPIO_RAW_INTR1),
	.rawchip_spi_freq = 27, /* MHz, should be the same to spi max_speed_hz */
	.rawchip_mclk_freq = 24, /* MHz, should be the same as cam csi0 mclk_clk_rate */
	.camera_rawchip_power_on = elite_rawchip_vreg_on,
	.camera_rawchip_power_off = elite_rawchip_vreg_off,
	.rawchip_use_ext_1v2 = elite_use_ext_1v2,
};

static struct platform_device msm_rawchip_device = {
	.name	= "rawchip",
	.dev	= {
		.platform_data = &msm_rawchip_board_info,
	},
};

#endif



static uint16_t msm_cam_gpio_tbl[] = {
	ELITE_GPIO_CAM_MCLK0, /*CAMIF_MCLK*/
	ELITE_GPIO_CAM_MCLK1,
#if 0
	ELITE_GPIO_CAM_I2C_DAT, /*CAMIF_I2C_DATA*/
	ELITE_GPIO_CAM_I2C_CLK, /*CAMIF_I2C_CLK*/
#endif
	ELITE_GPIO_RAW_INTR0,
	ELITE_GPIO_RAW_INTR1,
	ELITE_GPIO_MCAM_SPI_CLK,
	ELITE_GPIO_MCAM_SPI_CS0,
	ELITE_GPIO_MCAM_SPI_DI,
	ELITE_GPIO_MCAM_SPI_DO,
};

static struct msm_camera_gpio_conf gpio_conf = {
	.cam_gpiomux_conf_tbl = NULL,
	.cam_gpiomux_conf_tbl_size = 0,
	.cam_gpio_tbl = msm_cam_gpio_tbl,
	.cam_gpio_tbl_size = ARRAY_SIZE(msm_cam_gpio_tbl),
};


static struct gpiomux_setting cam_settings[16] = {
	{
		.func = GPIOMUX_FUNC_GPIO, /*suspend*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 1 - FUNC1 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 2*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 3 - FUNC1 8MA*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_2, /*active 4 - FUNC2 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 5 - I(L) 4MA*/
		.drv = GPIOMUX_DRV_4MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_IN,
	},

	{
		.func = GPIOMUX_FUNC_2, /*active 6 - A FUNC2 4MA*/
		.drv = GPIOMUX_DRV_4MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 7 - I(NP) 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_IN,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 8 - I(PD) 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_IN,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 9 - O(H) 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_HIGH,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 10 - O(L) 8MA*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 11 - I(PU) 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_UP,
		.dir = GPIOMUX_IN,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 12 - O(L) 2MA*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},

	{
		.func = GPIOMUX_FUNC_2, /*active 13 - A FUNC2 8MA*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 14 - I(NP) 8MA*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_IN,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 15 - I(PD) 8MA*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_IN,
	},
};

static struct msm_gpiomux_config elite_cam_configs[] = {
	{
		.gpio = ELITE_GPIO_CAM_MCLK1,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[13], /*A FUNC2 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[10], /*O(L) 8MA*/
		},
	},
	{
		.gpio = ELITE_GPIO_CAM_MCLK0,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],  /*Fun1 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[10], /*O(L) 8MA*/
		},
	},

	{
		.gpio = ELITE_GPIO_CAM_PWDN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],       /*O(H) 2MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[12],  /*O(L) 2MA*/
		},
	},

	{
		.gpio = ELITE_GPIO_CAM_I2C_DAT,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3], /*FUNC1 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /*I(PD) 8MA*/
		},
	},
	{
		.gpio = ELITE_GPIO_CAM_I2C_CLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3], /*FUNC1 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /*I(PD) 8MA*/
		},
	},
	{
		.gpio = ELITE_GPIO_RAW_INTR0,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[7], /*I(NP) 2MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[8], /*I(PD) 2MA*/
		},
	},
	{
		.gpio = ELITE_GPIO_RAW_INTR1,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[7], /*I(NP) 2MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[8], /*I(PD) 2MA*/
		},
	},
	/* gpio config for Rawchip SPI - gsbi10 */
	{
		.gpio      = ELITE_GPIO_MCAM_SPI_CLK,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_settings[13], /*A FUNC2 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /* I(PD) 8MA */
		},
	},
	{
		.gpio      = ELITE_GPIO_MCAM_SPI_CS0,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_settings[13], /*A FUNC2 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /* I(PD) 8MA */
		},
	},
	{
		.gpio      = ELITE_GPIO_MCAM_SPI_DI,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_settings[13], /*A FUNC2 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /* I(PD) 8MA */
		},
	},
	{
		.gpio      = ELITE_GPIO_MCAM_SPI_DO,
		.settings = {
			[GPIOMUX_ACTIVE] = &cam_settings[13], /*A FUNC2 8MA*/
			[GPIOMUX_SUSPENDED] = &cam_settings[15], /* I(PD) 8MA */
		},
	},
};

static struct msm_bus_vectors cam_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_preview_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 96215040,
		.ib  = 378224640,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 342150912,
		.ib  = 1361968128,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 207747072,
		.ib  = 489756672,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 60318720,
		.ib  = 150796800,
	},
};

static struct msm_bus_vectors cam_snapshot_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 147045888,
		.ib  = 588183552,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 263678976,
		.ib  = 659197440,
	},
};

static struct msm_bus_vectors cam_zsl_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 319044096,
		.ib  = 1271531520,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 239708160,
		.ib  = 599270400,
	},
};

static struct msm_bus_paths cam_bus_client_config[] = {
	{
		ARRAY_SIZE(cam_init_vectors),
		cam_init_vectors,
	},
	{
		ARRAY_SIZE(cam_preview_vectors),
		cam_preview_vectors,
	},
	{
		ARRAY_SIZE(cam_video_vectors),
		cam_video_vectors,
	},
	{
		ARRAY_SIZE(cam_snapshot_vectors),
		cam_snapshot_vectors,
	},
	{
		ARRAY_SIZE(cam_zsl_vectors),
		cam_zsl_vectors,
	},
};

static struct msm_bus_scale_pdata cam_bus_client_pdata = {
		cam_bus_client_config,
		ARRAY_SIZE(cam_bus_client_config),
		.name = "msm_camera",
};

static struct regulator *reg_8921_l2;
static struct regulator *reg_8921_l8;
static struct regulator *reg_8921_l9;
static struct regulator *reg_8921_lvs5;
static struct regulator *reg_8921_lvs6;

static int camera_sensor_power_enable(char *power, unsigned volt, struct regulator **sensor_power)
{
	int rc;

	if (power == NULL)
		return -ENODEV;

	*sensor_power = regulator_get(NULL, power);

	if (IS_ERR(*sensor_power)) {
		pr_err("[CAM] %s: Unable to get %s\n", __func__, power);
		return -ENODEV;
	}

	if (volt != 1800000) {
		rc = regulator_set_voltage(*sensor_power, volt, volt);
		if (rc < 0) {
			pr_err("[CAM] %s: unable to set %s voltage to %d rc:%d\n",
					__func__, power, volt, rc);
			regulator_put(*sensor_power);
			*sensor_power = NULL;
			return -ENODEV;
		}
	}

	rc = regulator_enable(*sensor_power);
	if (rc < 0) {
		pr_err("[CAM] %s: Enable regulator %s failed\n", __func__, power);
		regulator_put(*sensor_power);
		*sensor_power = NULL;
		return -ENODEV;
	}

	return rc;
}

static int camera_sensor_power_disable(struct regulator *sensor_power)
{

	int rc;
	if (sensor_power == NULL)
		return -ENODEV;

	if (IS_ERR(sensor_power)) {
		pr_err("[CAM] %s: Invalid requlator ptr\n", __func__);
		return -ENODEV;
	}

	rc = regulator_disable(sensor_power);
	if (rc < 0)
		pr_err("[CAM] %s: disable regulator failed\n", __func__);

	regulator_put(sensor_power);
	sensor_power = NULL;
	return rc;
}

static int elite_csi_vreg_on(void)
{
	pr_info("%s\n", __func__);
	return camera_sensor_power_enable("8921_l2", 1200000, &reg_8921_l2);
}

static int elite_csi_vreg_off(void)
{
	pr_info("%s\n", __func__);
	return camera_sensor_power_disable(reg_8921_l2);
}

struct msm_camera_device_platform_data msm_camera_csi_device_data[] = {
	{
		.ioclk.mclk_clk_rate = 24000000,
		.ioclk.vfe_clk_rate  = 228570000,
		.csid_core = 0,
		.camera_csi_on = elite_csi_vreg_on,
		.camera_csi_off = elite_csi_vreg_off,
		.cam_bus_scale_table = &cam_bus_client_pdata,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
	},
	{
		.ioclk.mclk_clk_rate = 24000000,
		.ioclk.vfe_clk_rate  = 228570000,
		.csid_core = 1,
		.camera_csi_on = elite_csi_vreg_on,
		.camera_csi_off = elite_csi_vreg_off,
		.cam_bus_scale_table = &cam_bus_client_pdata,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
	},
};

#ifdef CONFIG_S5K3H2YX
static int elite_s5k3h2yx_vreg_on(void)
{
	int rc;
	pr_info("[CAM] %s\n", __func__);

	/* VCM */
	rc = camera_sensor_power_enable("8921_l9", 2800000, &reg_8921_l9);
	if (rc < 0) {
		pr_err("[CAM] sensor_power_enable\
			(\"8921_l9\", 2.8V) FAILED %d\n", rc);
		goto enable_vcm_fail;
	}
	mdelay(1);

	/* analog */
	rc = camera_sensor_power_enable("8921_l8", 2800000, &reg_8921_l8);
	if (rc < 0) {
		pr_err("[CAM] sensor_power_enable\
			(\"8921_l8\", 2.8V) FAILED %d\n", rc);
		goto enable_analog_fail;
	}
	mdelay(1);

	if (system_rev == 0 || !elite_use_ext_1v2()) { /* for XA || XB wo ext1v2 */
	/* digital */
	rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "CAM_D1V2_EN");
	if (rc) {
		pr_err("[CAM] sensor_power_enable\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_CAM_D1V2_EN, rc);
		goto enable_digital_fail;
	}
	gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 1);
	gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
	mdelay(1);
	}

	/* IO */
	rc = camera_sensor_power_enable("8921_lvs6", 1800000, &reg_8921_lvs6);
	if (rc < 0) {
		pr_err("[CAM] sensor_power_enable\
			(\"8921_lvs6\", 1.8V) FAILED %d\n", rc);
		goto enable_io_fail;
	}

	return rc;

enable_io_fail:
	if (system_rev == 0 || !elite_use_ext_1v2()) { /* for XA || XB wo ext1v2 */
	rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "CAM_D1V2_EN");
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_CAM_D1V2_EN, rc);
	else {
		gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 0);
		gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
	}
	}
enable_digital_fail:
	camera_sensor_power_disable(reg_8921_l8);
enable_analog_fail:
	camera_sensor_power_disable(reg_8921_l9);
enable_vcm_fail:
	return rc;
}

static int elite_s5k3h2yx_vreg_off(void)
{
	int rc = 0;

	pr_info("[CAM] %s\n", __func__);

	/* analog */
	rc = camera_sensor_power_disable(reg_8921_l8);
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable\
			(\"8921_l8\") FAILED %d\n", rc);
	mdelay(1);

	if (system_rev == 0 || !elite_use_ext_1v2()) { /* for XA || XB wo ext1v2 */
	/* digital */
	rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "CAM_D1V2_EN");
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable\
			(\"gpio %d\", 1.2V) FAILED %d\n",
			ELITE_GPIO_V_CAM_D1V2_EN, rc);
	else {
		gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 0);
		gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
	}
	mdelay(1);
	}

	/* IO */
	rc = camera_sensor_power_disable(reg_8921_lvs6);
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable\
			(\"8921_lvs6\") FAILED %d\n", rc);

	mdelay(1);

	/* VCM */
	rc = camera_sensor_power_disable(reg_8921_l9);
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable\
			(\"8921_l9\") FAILED %d\n", rc);

	return rc;
}

#ifdef CONFIG_S5K3H2YX_ACT
static struct i2c_board_info s5k3h2yx_actuator_i2c_info = {
	I2C_BOARD_INFO("s5k3h2yx_act", 0x11),
};

static struct msm_actuator_info s5k3h2yx_actuator_info = {
	.board_info     = &s5k3h2yx_actuator_i2c_info,
	.bus_id         = MSM_8960_GSBI4_QUP_I2C_BUS_ID,
	.vcm_pwd        = ELITE_GPIO_CAM_VCM_PD,
	.vcm_enable     = 1,
};
#endif

static struct msm_camera_csi_lane_params s5k3h2yx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_s5k3h2yx_board_info = {
	.mount_angle = 90,
	.sensor_reset_enable = 0,
	.sensor_reset	= 0,
	.sensor_pwd	= ELITE_GPIO_CAM_PWDN,
	.vcm_pwd	= ELITE_GPIO_CAM_VCM_PD,
	.vcm_enable	= 1,
        .csi_lane_params = &s5k3h2yx_csi_lane_params,
};

/* Andrew_Cheng linear led 20111205 MB */ // Mu Lee sync EVA 20120127
/*
150 mA FL_MODE_FLASH_LEVEL1
200 mA FL_MODE_FLASH_LEVEL2
300 mA FL_MODE_FLASH_LEVEL3
400 mA FL_MODE_FLASH_LEVEL4
500 mA FL_MODE_FLASH_LEVEL5
600 mA FL_MODE_FLASH_LEVEL6
700 mA FL_MODE_FLASH_LEVEL7
*/
static struct camera_led_est msm_camera_sensor_s5k3h2yx_led_table[] = {
//		{
//		.enable = 0,
//		.led_state = FL_MODE_FLASH_LEVEL1,
//		.current_ma = 150,
//		.lumen_value = 150,
//		.min_step = 50,
//		.max_step = 70
//	},
		{
		.enable = 1,
		.led_state = FL_MODE_FLASH_LEVEL2,
		.current_ma = 200,
		.lumen_value = 250,
		.min_step = 64,//31,/*26, //Mu L 0209 */
		.max_step = 256
	},
		{
		.enable = 1,//0, Mu L 0302
		.led_state = FL_MODE_FLASH_LEVEL3,
		.current_ma = 300,
		.lumen_value = 350,//300,
		.min_step = 60,//29,
		.max_step = 63//34
	},
		{
		.enable = 1,//0, Mu L 0302
		.led_state = FL_MODE_FLASH_LEVEL4,
		.current_ma = 400,
		.lumen_value = 440,//400,
		.min_step = 56,//27,
		.max_step = 59//28
	},
//		{
//		.enable = 0,
//		.led_state = FL_MODE_FLASH_LEVEL5,
//		.current_ma = 500,
//		.lumen_value = 500,
//		.min_step = 25,
//		.max_step = 26
//	},
		{
		.enable = 1,//0, Mu L 0302
		.led_state = FL_MODE_FLASH_LEVEL6,
		.current_ma = 600,
		.lumen_value = 625,//600,
		.min_step = 52,//23,
		.max_step = 55//24
	},
	/*
		{
		.enable = 0,
		.led_state = FL_MODE_FLASH_LEVEL7,
		.current_ma = 700,
		.lumen_value = 700,
		.min_step = 21,
		.max_step = 22
	},
	*/
		{
		.enable = 1,
		.led_state = FL_MODE_FLASH,
		.current_ma = 750,
		.lumen_value = 745,//725,   //mk0217
		.min_step = 0,
		.max_step = 51//30/*25    //Mu L 0209*/
	},

		{
		.enable = 2,
		.led_state = FL_MODE_FLASH_LEVEL2,
		.current_ma = 200,
		.lumen_value = 250,//245,  //mk0127
		.min_step = 0,
		.max_step = 270
	},
		{
		.enable = 0,
		.led_state = FL_MODE_FLASH_LEVEL3,
		.current_ma = 300,
		.lumen_value = 300,
		.min_step = 0,
		.max_step = 100
	},
		{
		.enable = 0,
		.led_state = FL_MODE_FLASH_LEVEL4,
		.current_ma = 400,
		.lumen_value = 400,
		.min_step = 101,
		.max_step = 200
	},
	{
		.enable = 0,
		.led_state = FL_MODE_FLASH_LEVEL7,
		.current_ma = 700,
		.lumen_value = 700,
		.min_step = 101,
		.max_step = 200
	},
		{
		.enable = 2,
		.led_state = FL_MODE_FLASH,
		.current_ma = 750,
		.lumen_value = 745,//725,   //mk0217
		.min_step = 271,
		.max_step = 325
	},
	{
		.enable = 0,
		.led_state = FL_MODE_FLASH_LEVEL5,
		.current_ma = 500,
		.lumen_value = 500,
		.min_step = 25,
		.max_step = 26
	},
		{
		.enable = 0,//3,  //mk0210
		.led_state = FL_MODE_FLASH,
		.current_ma = 750,
		.lumen_value = 750,//740,//725,
		.min_step = 271,
		.max_step = 325
	},
};

static struct camera_led_info msm_camera_sensor_s5k3h2yx_led_info = {
	.enable = 1,
	.low_limit_led_state = FL_MODE_TORCH,
	.max_led_current_ma = 750,
	.num_led_est_table = ARRAY_SIZE(msm_camera_sensor_s5k3h2yx_led_table),
};

static struct camera_flash_info msm_camera_sensor_s5k3h2yx_flash_info = {
	.led_info = &msm_camera_sensor_s5k3h2yx_led_info,
	.led_est_table = msm_camera_sensor_s5k3h2yx_led_table,
};

static struct camera_flash_cfg msm_camera_sensor_s5k3h2yx_flash_cfg = {
	.low_temp_limit		= 5,
	.low_cap_limit		= 15,
	.flash_info             = &msm_camera_sensor_s5k3h2yx_flash_info,
};
/* Andrew_Cheng linear led 20111205 ME */

static struct msm_camera_sensor_flash_data flash_s5k3h2yx = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
#ifdef CONFIG_MSM_CAMERA_FLASH
	.flash_src = &msm_flash_src,
#endif
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3h2yx_data = {
	.sensor_name	= "s5k3h2yx",
	.camera_power_on = elite_s5k3h2yx_vreg_on,
	.camera_power_off = elite_s5k3h2yx_vreg_off,
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_s5k3h2yx,
	.sensor_platform_info = &sensor_s5k3h2yx_board_info,
	.gpio_conf = &gpio_conf,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
#ifdef CONFIG_S5K3H2YX_ACT
	.actuator_info = &s5k3h2yx_actuator_info,
#endif
	.use_rawchip = RAWCHIP_ENABLE,
	.flash_cfg = &msm_camera_sensor_s5k3h2yx_flash_cfg, /* Andrew_Cheng linear led 20111205 */
};
#endif

#ifdef CONFIG_S5K6A1GX
#define ELITE_XB_GPIO_V_CAM2_D1V2_EN 93
static int elite_s5k6a1gx_vreg_on(void)
{
	int rc;
	pr_info("[CAM] %s\n", __func__);

	/* analog */
	rc = camera_sensor_power_enable("8921_l8", 2800000, &reg_8921_l8);
	pr_info("[CAM] sensor_power_enable(\"8921_l8\", 2.8V) == %d\n", rc);
	if (rc < 0) {
		pr_err("[CAM] sensor_power_enable(\"8921_l8\", 2.8V) FAILED %d\n", rc);
		goto enable_analog_fail;
	}
	udelay(50);

	/* IO */
	if (system_rev >= 1) { /* for XB */
		rc = camera_sensor_power_enable("8921_lvs6", 1800000, &reg_8921_lvs6);
		if (rc < 0) {
			pr_err("[CAM] sensor_power_enable\
				(\"8921_lvs6\", 1.8V) FAILED %d\n", rc);
			goto enable_io_fail;
		}
	} else { /* for XA */
		rc = camera_sensor_power_enable("8921_lvs5", 1800000, &reg_8921_lvs5);
		pr_info("[CAM] sensor_power_enable(\"8921_lvs5\", 1.8V) == %d\n", rc);
		if (rc < 0) {
			pr_err("[CAM] sensor_power_enable(\"8921_lvs5\", 1.8V) FAILED %d\n", rc);
			goto enable_io_fail;
		}

	}
	udelay(50);

	if (system_rev == 0) { /* for XA */
		/* tmp for XA : i2c pull up */
		rc = camera_sensor_power_enable("8921_lvs6", 1800000, &reg_8921_lvs6);
		if (rc < 0) {
			pr_err("[CAM] sensor_power_enable\
				(\"8921_lvs6\", 1.8V) FAILED %d\n", rc);
			goto enable_i2c_pullup_fail;
		}
	}

	/* reset pin */
	rc = gpio_request(ELITE_GPIO_CAM2_RSTz, "s5k6a1gx");
	pr_info("[CAM]reset pin gpio_request,%d\n", ELITE_GPIO_CAM2_RSTz);
	if (rc < 0) {
		pr_err("GPIO(%d) request failed", ELITE_GPIO_CAM2_RSTz);
		goto enable_rst_fail;
	}
	gpio_direction_output(ELITE_GPIO_CAM2_RSTz, 1);
	gpio_free(ELITE_GPIO_CAM2_RSTz);
	udelay(50);

	/* digital */
	if (system_rev >= 1) { /* for XB */
		rc = gpio_request(ELITE_XB_GPIO_V_CAM2_D1V2_EN, "s5k6a1gx");
		pr_info("[CAM]reset pin gpio_request,%d\n", ELITE_XB_GPIO_V_CAM2_D1V2_EN);
		if (rc < 0) {
			pr_err("GPIO(%d) request failed", ELITE_XB_GPIO_V_CAM2_D1V2_EN);
			goto enable_digital_fail;
		}
		gpio_direction_output(ELITE_XB_GPIO_V_CAM2_D1V2_EN, 1);
		gpio_free(ELITE_XB_GPIO_V_CAM2_D1V2_EN);
	} else { /* for XA */
		rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "s5k6a1gx");
		pr_info("[CAM]digital gpio_request,%d\n", ELITE_GPIO_V_CAM_D1V2_EN);
		if (rc < 0) {
			pr_err("GPIO(%d) request failed", ELITE_GPIO_V_CAM_D1V2_EN);
			goto enable_digital_fail;
		}
		gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 1);
		gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
	}
	udelay(50);

	return rc;

enable_digital_fail:
	rc = gpio_request(ELITE_GPIO_CAM2_RSTz, "s5k6a1gx");
	if (rc < 0)
		pr_err("GPIO(%d) request failed", ELITE_GPIO_CAM2_RSTz);
	else {
		gpio_direction_output(ELITE_GPIO_CAM2_RSTz, 0);
		gpio_free(ELITE_GPIO_CAM2_RSTz);
	}
enable_rst_fail:
	camera_sensor_power_disable(reg_8921_lvs6);
enable_i2c_pullup_fail:
	if (system_rev < 1) /* for XA */
	camera_sensor_power_disable(reg_8921_lvs5);
enable_io_fail:
	camera_sensor_power_disable(reg_8921_l8);
enable_analog_fail:
	return rc;
}

static int elite_s5k6a1gx_vreg_off(void)
{
	int rc;
	pr_info("[CAM] %s\n", __func__);

	/* digital */
	if (system_rev >= 1) { /* for XB */
		rc = gpio_request(ELITE_XB_GPIO_V_CAM2_D1V2_EN, "s5k6a1gx");
		if (rc < 0)
			pr_err("GPIO(%d) request failed", ELITE_XB_GPIO_V_CAM2_D1V2_EN);
		else {
			gpio_direction_output(ELITE_XB_GPIO_V_CAM2_D1V2_EN, 0);
			gpio_free(ELITE_XB_GPIO_V_CAM2_D1V2_EN);
		}
	} else { /* for XA */
		rc = gpio_request(ELITE_GPIO_V_CAM_D1V2_EN, "s5k6a1gx");
		pr_info("[CAM]digital gpio_request,%d\n", ELITE_GPIO_V_CAM_D1V2_EN);
		if (rc < 0)
			pr_err("GPIO(%d) request failed", ELITE_GPIO_V_CAM_D1V2_EN);
		else {
			gpio_direction_output(ELITE_GPIO_V_CAM_D1V2_EN, 0);
			gpio_free(ELITE_GPIO_V_CAM_D1V2_EN);
		}
	}
	udelay(50);

	/* reset pin */
	rc = gpio_request(ELITE_GPIO_CAM2_RSTz, "s5k6a1gx");
	pr_info("[CAM]reset pin gpio_request,%d\n", ELITE_GPIO_CAM2_RSTz);
	if (rc < 0)
		pr_err("GPIO(%d) request failed", ELITE_GPIO_CAM2_RSTz);
	else {
		gpio_direction_output(ELITE_GPIO_CAM2_RSTz, 0);
		gpio_free(ELITE_GPIO_CAM2_RSTz);
	}
	udelay(50);

	if (system_rev == 0) { /* for XA */
		/* tmp for XA : i2c pull up */
		rc = camera_sensor_power_disable(reg_8921_lvs6);
		if (rc < 0)
			pr_err("[CAM] sensor_power_disable\
				(\"8921_lvs6\") FAILED %d\n", rc);
		mdelay(1);
	}

	/* IO */
	if (system_rev >= 1) { /* for XB */
		rc = camera_sensor_power_disable(reg_8921_lvs6);
		if (rc < 0)
			pr_err("[CAM] sensor_power_disable(\"8921_lvs6\") FAILED %d\n", rc);
	} else { /* for XA */
		rc = camera_sensor_power_disable(reg_8921_lvs5);
		if (rc < 0)
			pr_err("[CAM] sensor_power_disable(\"8921_lvs5\") FAILED %d\n", rc);
	}
	udelay(50);

	/* analog */
	rc = camera_sensor_power_disable(reg_8921_l8);
	if (rc < 0)
		pr_err("[CAM] sensor_power_disable(\"8921_l8\") FAILED %d\n", rc);
	udelay(50);

	return rc;
}

static struct msm_camera_csi_lane_params s5k6a1gx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_s5k6a1gx_board_info = {
	.mount_angle = 270,
	.mirror_flip = CAMERA_SENSOR_NONE,
	.sensor_reset_enable = 0,
	.sensor_reset	= ELITE_GPIO_CAM2_RSTz,
	.sensor_pwd	= ELITE_GPIO_CAM2_STANDBY,
	.vcm_pwd	= 0,
	.vcm_enable	= 0,
	.csi_lane_params = &s5k6a1gx_csi_lane_params,
};

static struct msm_camera_sensor_flash_data flash_s5k6a1gx = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k6a1gx_data = {
	.sensor_name	= "s5k6a1gx",
	.sensor_reset	= ELITE_GPIO_CAM2_RSTz,
	.sensor_pwd	= ELITE_GPIO_CAM2_STANDBY,
	.vcm_pwd	= 0,
	.vcm_enable	= 0,
	.camera_power_on = elite_s5k6a1gx_vreg_on,
	.camera_power_off = elite_s5k6a1gx_vreg_off,
	.pdata	= &msm_camera_csi_device_data[1],
	.flash_data	= &flash_s5k6a1gx,
	.sensor_platform_info = &sensor_s5k6a1gx_board_info,
	.gpio_conf = &gpio_conf,
	.csi_if	= 1,
	.camera_type = FRONT_CAMERA_2D,
	.use_rawchip = RAWCHIP_DISABLE,
};
#endif

#ifdef CONFIG_MSM_CAMERA
struct i2c_board_info elite_camera_i2c_boardinfo[] = {
#ifdef CONFIG_S5K3H2YX
	{
	I2C_BOARD_INFO("s5k3h2yx", 0x20 >> 1),
	.platform_data = &msm_camera_sensor_s5k3h2yx_data,
	},
#endif
#ifdef CONFIG_S5K6A1GX
	{
	I2C_BOARD_INFO("s5k6a1gx", 0x6C >> 1),
	.platform_data = &msm_camera_sensor_s5k6a1gx_data,
	},
#endif
#ifdef CONFIG_MSM_CAMERA_FLASH_SC628A
	{
	I2C_BOARD_INFO("sc628a", 0x6E),
	},
#endif
};

struct msm_camera_board_info elite_camera_board_info = {
	.board_info = elite_camera_i2c_boardinfo,
	.num_i2c_board_info = ARRAY_SIZE(elite_camera_i2c_boardinfo),
};
#endif

static struct platform_device msm_camera_server = {
	.name = "msm_cam_server",
	.id = 0,
};

void __init elite_init_camera(void)
{
#ifdef CONFIG_MSM_CAMERA
	msm_gpiomux_install(elite_cam_configs,
			ARRAY_SIZE(elite_cam_configs));

        platform_device_register(&msm_rawchip_device);
	platform_device_register(&msm_camera_server);
	platform_device_register(&msm8960_device_i2c_mux_gsbi4);
	platform_device_register(&msm8960_device_csiphy0);
	platform_device_register(&msm8960_device_csiphy1);
	platform_device_register(&msm8960_device_csid0);
	platform_device_register(&msm8960_device_csid1);
	platform_device_register(&msm8960_device_ispif);
	platform_device_register(&msm8960_device_vfe);
	platform_device_register(&msm8960_device_vpe);
#endif
}
#endif

