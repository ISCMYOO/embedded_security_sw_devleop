CFG_TEE_TA_LOG_LEVEL ?= 4
CFG_TA_OPTEE_CORE_API_COMPAT_1_1=y
CFG_RPMB_FS=y

BINARY=22222222-2222-2222-2222-222222222222

LIBGCC_PATH := /home/iscmyoo/project/mobility_security/optee_stm32/Developer_Package/SDK/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/lib/arm-ostl-linux-gnueabi/13.3.0/libgcc.a

override libgccta_arm32 := $(LIBGCC_PATH)

-include /home/iscmyoo/project/mobility_security/optee_stm32/Developer_Package/stm32mp-openstlinux-6.6-yocto-scarthgap-mpu-v25.08.27/sources/ostl-linux/optee-os-stm32mp-4.0.0-stm32mp-r2-r0/build/optee-stm32mp157d-dk1/export-ta_arm32/mk/ta_dev_kit.mk

addclean:
	@rm -f *.o *.cmd *.d *.map *.dmp *.stripped.elf *.elf *.ta *.ld.d
