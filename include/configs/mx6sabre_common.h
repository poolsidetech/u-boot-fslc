/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 */

#ifndef __MX6QSABRE_COMMON_CONFIG_H
#define __MX6QSABRE_COMMON_CONFIG_H

#include <linux/stringify.h>

#include "mx6_common.h"

/* MMC Configs */
#define CFG_SYS_FSL_ESDHC_ADDR      0

#ifdef CONFIG_SUPPORT_EMMC_BOOT
#define EMMC_ENV \
	"emmcdev=2\0" \
	"update_emmc_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if ${get_cmd} ${update_sd_firmware_filename}; then " \
			"if mmc dev ${emmcdev} 1; then "	\
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0"
#else
#define EMMC_ENV ""
#endif

#define VIDEO_ARGS        "${video_args}"
#define VIDEO_ARGS_SCRIPT "run video_args_script; "

#define CONFIG_PREBOOT \
	"if hdmidet; then " \
		"setenv video_interfaces hdmi lvds; " \
	"else " \
		"setenv video_interfaces lvds hdmi; " \
	"fi;"

#define CFG_EXTRA_ENV_SETTINGS \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"fdtfile=imx6dl-sabresd.dtb\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"silent=0\0" \
	"console=" CONSOLE_DEV "\0" \
	"dfuspi=dfu 0 sf 0:0:10000000:0\0" \
	"dfu_alt_info_spl=spl raw 0x400\0" \
	"dfu_alt_info_img=u-boot raw 0x10000\0" \
	"dfu_alt_info=spl raw 0x400\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"splashimage=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"desired_boot_part_num=2\0" \
	"last_good_boot_part_num=2\0" \
	"trying_to_boot_part_num=0\0" \
	"detectnewos=" \
		"if test ${desired_boot_part_num} != ${last_good_boot_part_num}; then " \
			"if test ${trying_to_boot_part_num} = ${desired_boot_part_num}; then " \
				"echo WARN: Failed boot of new OS detected. reverting. ; " \
				"setenv desired_boot_part_num ${last_good_boot_part_num}; " \
			"else " \
				"echo New OS detected. let us try it. ; " \
				"setenv trying_to_boot_part_num ${desired_boot_part_num}; " \
			"fi; " \
		"fi\0" \
	"finduuid=part uuid mmc ${mmcdev}:${desired_boot_part_num} uuid\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	EMMC_ENV	  \
	"video_args_hdmi=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=hdmi,1280x720M@60,if=RGB24\0" \
	"video_args_lvds=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=ldb,LDB-XGA,if=RGB666\0" \
	"video_args_lcd=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=lcd,CLAA-WVGA,if=RGB666\0" \
	"fb=0\0" \
	"video_args_script=" \
		"for v in ${video_interfaces}; do " \
			"run video_args_${v}; " \
			"setexpr fb $fb + 1; " \
		"done\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=PARTUUID=${uuid} fec.disable_giga=1 rootwait rw " \
		VIDEO_ARGS "\0" \
	"loadbootscript=" \
		"ext4load mmc ${mmcdev}:${desired_boot_part_num} ${loadaddr} ${script} || " \
		"ext4load mmc ${mmcdev}:${desired_boot_part_num} ${loadaddr} boot/${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
    "loadimage=ext4load mmc ${mmcdev}:${desired_boot_part_num} ${loadaddr} /boot/${image}\0" \
    "loadfdt=ext4load mmc ${mmcdev}:${desired_boot_part_num} ${fdt_addr} /boot/${fdtfile}\0" \
	"mmcboot=echo Booting from mmc ...; " \
        "run detectnewos; " \
		"run finduuid; " \
		VIDEO_ARGS_SCRIPT \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdtfile}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
		"findfdt="\
			"if test $fdtfile = undefined; then " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6QP; then " \
					"setenv fdtfile imx6qp-sabreauto.dtb; fi; " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6Q; then " \
					"setenv fdtfile imx6q-sabreauto.dtb; fi; " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6DL; then " \
					"setenv fdtfile imx6dl-sabreauto.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6QP; then " \
					"setenv fdtfile imx6qp-sabresd.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6Q; then " \
					"setenv fdtfile imx6q-sabresd.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6DL; then " \
					"setenv fdtfile imx6dl-sabresd.dtb; fi; " \
				"if test $fdtfile = undefined; then " \
					"echo WARNING: Could not determine dtb to use; fi; " \
			"fi;\0" \

/* Physical Memory Map */
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CFG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CFG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CFG_SYS_INIT_RAM_SIZE       IRAM_SIZE

/* Environment organization */

#endif                         /* __MX6QSABRE_COMMON_CONFIG_H */
