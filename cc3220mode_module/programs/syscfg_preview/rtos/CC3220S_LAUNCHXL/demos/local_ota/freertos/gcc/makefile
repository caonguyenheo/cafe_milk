SIMPLELINK_CC32XX_SDK_INSTALL_DIR ?= $(abspath ../../../../../../../..)

include $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/kernel/freertos/builds/CC3220S_LAUNCHXL/release
SYSCONFIG_PRODUCT := $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/.metadata/sdk.json
BOARD := /ti/boards/.meta/CC3220S_LAUNCHXL

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = Board.obj uart_term.obj main_freertos.obj ota_report_server_task.obj local_ota_task.obj local_ota.obj ota_archive.obj link_local_task.obj platform.obj ota_json.obj

NAME = local_ota

CFLAGS = -I../.. \
    -I. \
    -DCC32XX \
    "-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/posix/gcc" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM3" \
    "-I$(KERNEL_BUILD)" \
    -mcpu=cortex-m4 \
    -march=armv7e-m \
    -mthumb \
    -std=c99 \
    -mfloat-abi=soft \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -gstrict-dwarf \
    -Wall \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include/newlib-nano" \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include"

LFLAGS = "-L$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/utils/json/lib" \
    -l:gcc/m4/json_release.a \
    -Wl,-T,../../freertos/gcc/CC3220S_LAUNCHXL_FREERTOS.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source" \
    -l:ti/display/lib/display.am4g \
    -l:ti/grlib/lib/gcc/m4/grlib.a \
    -l:third_party/spiffs/lib/gcc/m4/spiffs.a \
    -l:ti/drivers/lib/drivers_cc32xx.am4g \
    -l:third_party/fatfs/lib/gcc/m4/fatfs.a \
    "-L$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/drivers/net/wifi" \
    -l:gcc/rtos/simplelink.a \
    "-L$(KERNEL_BUILD)/gcc" \
    -l:freertos.lib \
    -l:ti/devices/cc32xx/driverlib/gcc/Release/driverlib.a \
    -march=armv7e-m \
    -mthumb \
    -nostartfiles \
    -static \
    -Wl,--gc-sections \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib/thumb/v7e-m" \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib" \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    --specs=nano.specs

all: $(NAME).out

$(KERNEL_BUILD)/gcc/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)
.INTERMEDIATE: syscfg
Board.c Board.h : syscfg
	@ echo generation complete

syscfg: ../../common.syscfg
	@ echo Generating configuration files...
	@ $(NODE_JS) $(SYSCONFIG_TOOL) --product $(SYSCONFIG_PRODUCT) --board $(BOARD) --output $(@D) $<

Board.obj: Board.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

uart_term.obj: ../../uart_term.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

main_freertos.obj: ../../freertos/main_freertos.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ota_report_server_task.obj: ../../ota_report_server_task.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

local_ota_task.obj: ../../local_ota_task.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

local_ota.obj: ../../local_ota.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ota_archive.obj: ../../ota_archive.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

link_local_task.obj: ../../link_local_task.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

platform.obj: ../../platform.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ota_json.obj: ../../ota_json.c Board.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/gcc/freertos.lib
	@ echo linking $@
	@ $(LNK)  $(OBJECTS) $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) Board.c Board.h  > $(DEVNULL) 2>&1
