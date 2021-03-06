/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <linux/kernel.h>
#include "Yushan_HTC_Functions.h"
/* #define AFSU_DEBUG */

uint8_t pdpclib_u_ar0260[] =
{
	0x01,
	0x01,
	0x01,
	0x88,
	0x07,
	0x40,
	0x04,
	0x00,
	0x20,
	0x00,
	0x00,
	0x02,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x20,
	0x00,
	0x00,
	0x01,
	0x00,
	0x02,
	0x00,
	0x04,
	0x00,
	0x08,
	0x00,
	0x10,
	0x80,
	0x00,
	0x00,
	0x01,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x00,
	0x80,
	0x00,
	0x00,
	0x00,
	0x40,
	0x00,
	0x00,
	0x00,
	0x20,
	0x00,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x02,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x02,
	0x02,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x03,
	0x03,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x03,
	0x03,
	0xf9,
	0x00,
	0x02,
	0x03,
	0x03,
	0x03,
	0xfa,
	0x00,
	0x04,
	0x06,
	0x05,
	0x05,
	0xfa,
	0x00,
	0x04,
	0x06,
	0x05,
	0x05,
	0xfa,
	0x00,
	0x04,
	0x06,
	0x05,
	0x05,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x02,
	0x00,
	0x08,
	0x00,
	0x10,
	0x78,
	0x01,
	0x00,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x01,
	0x11,
	0x21,
	0xd6,
	0x01,
	0x40,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x04,
	0x1f,
	0x3a,
	0xd6,
	0x01,
	0x40,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x01,
	0x2c,
	0x57,
	0x78,
	0x01,
	0x00,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x00,
	0x08,
	0x10,
	0xd6,
	0x01,
	0x40,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x02,
	0x10,
	0x1d,
	0xd6,
	0x01,
	0x40,
	0x01,
	0x60,
	0x02,
	0x50,
	0x03,
	0x04,
	0x01,
	0x16,
	0x2b,
	0xab,
	0x0f,
	0x00,
	0x00,
	0xab,
	0x0a,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x80,
	0x00,
	0x00,
	0x00,
	0x55,
	0x02,
	0x00,
	0x00,
	0x2b,
	0x04,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xa0,
	0xff,
	0xff,
	0xff,
	0xa0,
	0x01,
	0x00,
	0x00,
	0xa0,
	0x03,
	0x00,
	0x00,
	0xab,
	0x0f,
	0x00,
	0x00,
	0xab,
	0x0a,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x55,
	0x00,
	0x00,
	0x00,
	0x55,
	0x01,
	0x00,
	0x00,
	0x2b,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xe0,
	0xff,
	0xff,
	0xff,
	0xc0,
	0x00,
	0x00,
	0x00,
	0xc0,
	0x01,
	0x00,
	0x00,
	0xff,
	0x01,
	0xff,
	0x05,

};

uint8_t dppclib_u_ar0260[] =
{
	0x01,
	0x01,
	0x01,
	0x88,
	0x07,
	0x40,
	0x04,
	0x00,
	0x05,
	0xfa,
	0x84,
	0x00,
	0xbc,
	0xfe,
	0x32,
	0x01,
	0x02,
	0x05,
	0x80,
	0x04,
	0x00,
	0x01,
	0x00,
	0x1b,
	0x00,
	0xfb,
	0x00,
	0x00,
	0xfc,
	0x00,
	0xfe,
	0x00,
	0x00,
	0x9c,
	0x04,
	0x0d,
	0x01,
	0xff,
	0x1c,
	0x12,
	0xfa,
	0xff,
	0x17,
	0xfc,
	0xf6,
	0xff,
	0xfb,
	0xf8,
	0x8a,
	0x04,
	0x09,
	0x01,
	0xff,
	0x1a,
	0x10,
	0xfb,
	0xff,
	0x04,
	0xfc,
	0xfc,
	0xfe,
	0xfb,
	0xfe,
	0x80,
	0xfe,
	0x00,
	0xff,
	0x00,
	0xf4,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0xff,
	0x00,
	0x00,
	0x6d,
	0xfd,
	0xfe,
	0xff,
	0x01,
	0xf2,
	0x0c,
	0x02,
	0x00,
	0xee,
	0x00,
	0x09,
	0xff,
	0xfc,
	0x08,
	0x74,
	0xff,
	0xff,
	0xff,
	0x01,
	0xf6,
	0x0f,
	0x03,
	0xff,
	0xf4,
	0x00,
	0x05,
	0x00,
	0xfc,
	0x05,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x03,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x04,

};

uint8_t dopclib_u_ar0260[] =
// default to use lib v1.1
{
	0x01,
	0x01,
	0x01,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x20,
	0x00,
	0x80,
	0x00,
	0x00,
	0x04,
	0x00,
	0x01,
	0x00,
	0x08,
	0x20,
	0x00,
	0x00,
	0x02,
	0x88,
	0x07,
	0x40,
	0x04,
	0x78,
	0x00,
	0x7a,
	0x00,
	0x5a,
	0x00,
	0x5c,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x10,
	0x0c,
	0x00,
	0xe6,
	0x65,
	0x43,
	0x32,
	0x22,
	0x65,
	0x43,
	0x22,
	0x11,
	0x54,
	0x32,
	0x21,
	0x11,
	0x43,
	0x22,
	0x11,
	0x00,
	0x43,
	0x21,
	0x10,
	0x00,
	0x43,
	0x21,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x22,
	0x22,
	0x22,
	0x22,
	0x22,
	0x22,
	0x22,
	0x22,
	0x22,
	0x21,
	0x11,
	0x11,
	0x22,
	0x21,
	0x11,
	0x11,
	0x22,
	0x21,
	0x11,
	0x00,
	0x22,
	0x21,
	0x11,
	0x00,
	0x01,
	0x01,
	0x01,
	0x01,
	0x00,
	0x00,
	0x01,
	0x01,
	0x00,
	0x03,
	0x02,
	0x01,
	0x0b,
	0x38,
	0x00,
	0x01,
	0x01,
	0x01,
	0x01,
	0xff,
	0xff,
	0xff,
	0xff,
	0x00,
	0x00,
	0x00,
	0x00,
	0x1e,
	0x00,
	0x06,
	0x0e,
	0x08,
	0x08,
	0x80,
	0x02,
	0x00,
	0x06,
	0x00,
	0x0a,
	0x9f,
	0x52,
	0x9f,
	0x33,
	0x9f,
	0x52,
	0x9f,
	0x33,
	0x9f,
	0x52,
	0x9f,
	0x33,
	0x9f,
	0x52,
	0x9f,
	0x43,
	0x80,
	0x02,
	0x00,
	0x06,
	0x00,
	0x0a,
	0x9f,
	0x4d,
	0x9f,
	0x66,
	0x9f,
	0x4d,
	0x9f,
	0x66,
	0x9f,
	0x4d,
	0x9f,
	0x66,
	0x9f,
	0x4d,
	0x9f,
	0x76,
	0x0c,
	0x00,
	0x14,
	0x00,
	0x3c,
	0x00,
	0x10,
	0x27,
	0x04,
	0x00,
	0x03,
	0x04,
	0x04,
	0x04,
	0x64,
	0x00,
	0x4c,
	0x04,
	0x00,
	0x01,
	0x12,
	0x12,
	0x0d,
	0x06,
	0x10,
	0x84,
	0x03,
	0x04,
	0x05,
	0x03,
	0x04,
	0x04,
	0x03,
	0x04,
	0x03,
	0x02,
	0x80,
	0x01,
	0x00,
	0x03,
	0x00,
	0x06,
	0x07,
	0x0c,
	0x11,
	0x06,
	0x0f,
	0x11,
	0x06,
	0x14,
	0x11,
	0x05,
	0x1a,
	0x11,
	0x00,
	0x00,
	0x00,
	0x02,
	0x80,
	0x02,
	0x23,
	0x23,
	0x7f,
	0x1d,
	0x1d,
	0x7f,
	0x05,
	0x05,
	0x00,
	0x00,
	0x00,
	0x00,
	0xb8,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xc4,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0a,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x7f,
	0x7f,
	0x00,
	0x00,
	0x01,
	0x80,
	0x02,
	0x00,
	0x06,
	0x00,
	0x0c,
	0x00,
	0x04,
	0x02,
	0x03,
	0x01,
	0x01,
	0x04,
	0x04,
	0x03,
	0x03,
	0x03,
	0x04,
	0x04,
	0x04,
	0x04,
	0x03,
	0x05,
	0x05,
	0x04,
	0x04,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x04,
	0x00,
	0x08,
	0x00,
	0x10,
	0x00,
	0x00,
	0xab,
	0xfe,
	0xc0,
	0xff,
	0xc0,
	0xff,
	0x00,
	0x00,
	0x0b,
	0x0b,
	0x07,
	0x06,
	0x04,
	0x80,
	0x02,
	0x00,
	0x06,
	0x00,
	0x0c,
	0x00,
	0x04,
	0x05,
	0x03,
	0x04,
	0x01,
	0x04,
	0x04,
	0x03,
	0x03,
	0x03,
	0x04,
	0x04,
	0x04,
	0x04,
	0x03,
	0x05,
	0x05,
	0x04,
	0x04,
	0x00,
	0x00,
	0x00,
	0x01,
	0x00,
	0x04,
	0x00,
	0x08,
	0x00,
	0x10,
	0x00,
	0x00,
	0xab,
	0xfc,
	0x00,
	0xff,
	0x80,
	0xff,
	0x00,
	0x00,
	0x1d,
	0x1d,
	0x13,
	0x0f,
	0x0b,
	0x65,
	0x54,
	0x33,
	0x32,
	0x65,
	0x43,
	0x32,
	0x22,
	0x54,
	0x33,
	0x22,
	0x11,
	0x54,
	0x32,
	0x21,
	0x11,
	0x43,
	0x32,
	0x11,
	0x10,
	0x43,
	0x22,
	0x11,
	0x00,
	0x01,
	0x01,
	0x01,
	0x02,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x09,
	0x1a,
	0x0f,
	0x01,
	0x01,
	0x0b,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x05,
	0x1a,
	0x14,
	0x02,
	0x05,
	0x0c,
	0x07,
	0x00,
	0x02,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0d,
	0x1a,
	0x14,
	0x02,
	0x04,
	0x0d,
	0x05,
	0x00,
	0x01,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0c,
	0x1a,
	0x13,
	0x00,
	0x00,
	0x00,
	0x02,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x26,
	0x40,
	0x00,
	0x00,
	0x02,
	0x03,
	0x06,
	0x03,
	0x04,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0a,
	0x26,
	0x1f,
	0x00,
	0x00,
	0x00,
	0x07,
	0x07,
	0x05,
	0x05,
	0x02,
	0x04,
	0x00,
	0x00,
	0x00,
	0x15,
	0x26,
	0x1e,
	0x00,
	0x03,
	0x04,
	0x06,
	0x06,
	0x05,
	0x05,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x11,
	0x26,
	0x1a,
	0x00,
	0x00,
	0x00,
	0x01,
	0x03,
	0x01,
	0x00,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,
	0x03,
	0x08,
	0x04,
	0x05,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x1d,
	0x40,
	0x3a,
	0x00,
	0x00,
	0x00,
	0x05,
	0x0a,
	0x07,
	0x08,
	0x02,
	0x04,
	0x01,
	0x00,
	0x00,
	0x27,
	0x40,
	0x34,
	0x00,
	0x00,
	0x00,
	0x05,
	0x09,
	0x07,
	0x08,
	0x01,
	0x03,
	0x01,
	0x00,
	0x00,
	0x20,
	0x40,
	0x2e,
	0x00,
	0x00,
	0x00,
	0x00,
	0x02,
	0x00,
	0x00,
	0x01,
	0x01,
	0x01,
	0x01,
	0x00,
	0x22,
	0x40,
	0x32,
	0x00,
	0x00,
	0x00,
	0x01,
	0x08,
	0x04,
	0x04,
	0x01,
	0x00,
	0x01,
	0x01,
	0x00,
	0x20,
	0x40,
	0x3b,
	0x00,
	0x00,
	0x00,
	0x02,
	0x09,
	0x07,
	0x08,
	0x00,
	0x04,
	0x03,
	0x01,
	0x00,
	0x20,
	0x40,
	0x31,
	0x00,
	0x00,
	0x00,
	0x01,
	0x08,
	0x07,
	0x08,
	0x01,
	0x03,
	0x03,
	0x01,
	0x00,
	0x24,
	0x40,
	0x32,
	0x01,
	0x01,
	0x02,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x1a,
	0x12,
	0x01,
	0x01,
	0x04,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0d,
	0x1a,
	0x17,
	0x01,
	0x02,
	0x08,
	0x01,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0b,
	0x1a,
	0x14,
	0x01,
	0x01,
	0x07,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0c,
	0x1a,
	0x15,
	0x00,
	0x00,
	0x00,
	0x00,
	0x02,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x26,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x04,
	0x02,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x0e,
	0x26,
	0x20,
	0x00,
	0x00,
	0x00,
	0x02,
	0x06,
	0x05,
	0x09,
	0x00,
	0x01,
	0x00,
	0x00,
	0x00,
	0x19,
	0x26,
	0x22,
	0x00,
	0x00,
	0x00,
	0x01,
	0x06,
	0x04,
	0x06,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x16,
	0x26,
	0x22,
	0x00,
	0x00,
	0x00,
	0x00,
	0x03,
	0x03,
	0x03,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x25,
	0x40,
	0x2d,
	0x00,
	0x00,
	0x00,
	0x00,
	0x05,
	0x03,
	0x05,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x2e,
	0x40,
	0x3d,
	0x00,
	0x00,
	0x00,
	0x01,
	0x08,
	0x06,
	0x09,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x30,
	0x40,
	0x3b,
	0x00,
	0x00,
	0x00,
	0x00,
	0x07,
	0x06,
	0x08,
	0x01,
	0x00,
	0x00,
	0x00,
	0x00,
	0x2f,
	0x40,
	0x3c,
	0x00,
	0x00,
	0x00,
	0x00,
	0x03,
	0x03,
	0x03,
	0x01,
	0x01,
	0x00,
	0x01,
	0x00,
	0x33,
	0x40,
	0x39,
	0x00,
	0x00,
	0x00,
	0x00,
	0x05,
	0x03,
	0x04,
	0x01,
	0x01,
	0x00,
	0x01,
	0x00,
	0x30,
	0x40,
	0x3e,
	0x00,
	0x00,
	0x00,
	0x00,
	0x08,
	0x06,
	0x09,
	0x01,
	0x01,
	0x01,
	0x01,
	0x00,
	0x32,
	0x40,
	0x3c,
	0x00,
	0x00,
	0x00,
	0x00,
	0x07,
	0x05,
	0x08,
	0x01,
	0x01,
	0x01,
	0x01,
	0x00,
	0x32,
	0x40,
	0x3d,
	0x14,
	0x2b,
	0x3f,
	0x14,
	0x1b,
	0x3f,
	0x3f,
	0x0f,
	0x3f,
	0x3f,
	0x20,
	0x14,
	0x14,
	0x2b,
	0x3f,
	0x14,
	0x1b,
	0x3f,
	0x3f,
	0x0f,
	0x3f,
	0x3f,
	0x20,
	0x14,
	0x00,
	0x00,
	0x00,
	0x04,
	0x00,
	0x08,
	0x00,
	0x00,
	0xc0,
	0xfa,
	0x00,
	0x00,
	0x00,
	0x00,
	0x40,
	0x07,
	0x00,
	0x00,
	0x15,
	0x15,
	0x00,
	0x1c,
	0x1c,
	0x39,
	0x36,
	0x80,
	0x01,
	0x00,
	0x03,
	0x00,
	0x06,
	0x00,
	0x0c,
	0x23,
	0x2d,
	0x31,
	0x29,
	0x16,
	0x18,
	0x16,
	0x2a,
	0x07,
	0x1b,
	0x2e,
	0x32,
	0x2a,
	0x11,
	0x15,
	0x11,
	0x2a,
	0x04,
	0x14,
	0x36,
	0x3d,
	0x35,
	0x0e,
	0x13,
	0x0e,
	0x36,
	0x02,
	0x0e,
	0x2a,
	0x36,
	0x30,
	0x09,
	0x10,
	0x09,
	0x32,
	0x01,
	0x0a,
	0x29,
	0x35,
	0x2f,
	0x04,
	0x0e,
	0x04,
	0x31,
	0x00,
	0x36,
	0x80,
	0x01,
	0x00,
	0x03,
	0x00,
	0x06,
	0x00,
	0x0c,
	0x3f,
	0x3f,
	0x3f,
	0x3f,
	0x16,
	0x18,
	0x16,
	0x3f,
	0x07,
	0x3f,
	0x33,
	0x37,
	0x31,
	0x11,
	0x15,
	0x11,
	0x31,
	0x05,
	0x33,
	0x2c,
	0x33,
	0x2b,
	0x0e,
	0x13,
	0x0e,
	0x2c,
	0x04,
	0x24,
	0x31,
	0x3c,
	0x35,
	0x09,
	0x10,
	0x09,
	0x37,
	0x02,
	0x19,
	0x30,
	0x3b,
	0x34,
	0x04,
	0x0e,
	0x04,
	0x36,
	0x01,
	0x40,
	0x00,
	0x02,
	0x00,
	0x06,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,

};


struct yushan_reg_clib_t yushan_regs_clib_ar0260 = {
	.pdpclib_first_addr = 0x0dd7,
	.pdpclib = &pdpclib_u_ar0260[0],
	.pdpclib_size = ARRAY_SIZE(pdpclib_u_ar0260),

	.dppclib_first_addr = 0xbe27,
	.dppclib = &dppclib_u_ar0260[0],
	.dppclib_size = ARRAY_SIZE(dppclib_u_ar0260),

// default to use lib v1.1
#if 1 //#ifdef CONFIG_USEDXOAF //lib v1.1
	.dopclib_first_addr = 0x3bcc,
#else
	.dopclib_first_addr = 0x3bcc,
#endif
	.dopclib = &dopclib_u_ar0260[0],
	.dopclib_size = ARRAY_SIZE(dopclib_u_ar0260),
};

