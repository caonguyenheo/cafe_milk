SIMPLELINK_CC32XX_SDK_INSTALL_DIR ?= $(abspath ../../../..)

include $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/kernel/tirtos/builds/CC3220SF_LAUNCHXL/release

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

#OBJECTS = uart_terminal.obj main_tirtos.obj CC3220SF_LAUNCHXL.obj xmlcreate.obj tcp.obj \
#		 shelltask.obj main_thread.obj osi_posix.obj network_handler.obj system.obj tftp.obj \
#		 sflash.obj userconfig.obj
		 
#OBJECTS = main_tirtos.obj CC3220SF_LAUNCHXL.obj main_thread.obj uart_terminal.obj tcp.obj \
#		shelltask.obj system.obj network_handler.obj sflash.obj userconfig.obj apconfig.obj cc3220_system.obj \
#		spi_handler.obj nxcGenUDID_24.obj osi_posix.obj xmlcreate.obj tftp.obj
		
#OBJECTS = main_tirtos.obj main_thread.obj uart_terminal.obj tcp.obj shelltask.obj system.obj \
#		network_handler.obj userconfig.obj sflash.obj apconfig.obj cc3220_system.obj \
#		spi_handler.obj spi_handler_temp.obj nxcGenUDID_24.obj osi_posix.obj xmlcreate.obj CC3220SF_LAUNCHXL.obj \

OBJECTS = main_tirtos.obj main_thread.obj uart_terminal.obj tcp.obj shelltask.obj system.obj \
		network_handler.obj userconfig.obj sflash.obj apconfig.obj cc3220_system.obj \
		spi_handler_temp.obj nxcGenUDID_24.obj osi_posix.obj xmlcreate.obj CC3220SF_LAUNCHXL.obj \


CONFIGPKG = $(KERNEL_BUILD)/ccs

NAME = p2p
#	"-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/programs/p2p/ftp"
CFLAGS = -I../.. \
	"-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/net/bsd" \
	"-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/programs/p2p/app" \
	"-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/programs/p2p/cc3220launch" \
    -DCC32XX \
    "-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/posix/ccs" \
    --silicon_version=7M4 \
    --code_state=16 \
    --little_endian \
    --display_error_number \
    --diag_warning=255 \
    --diag_wrap=off \
    --gen_func_subsections=on \
    --float_support=vfplib \
    --symdebug:dwarf \
    "-I$(CCS_ARMCOMPILER)/include"

LFLAGS = "-i$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/net/lib" \
    -lccs/m4/slnetsock_release.a \
    "-i$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/drivers/net/wifi/slnetif" \
    -lccs/Release/slnetifwifi.a \
	"-i$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source" \
    -lti/display/lib/display.aem4 \
    -lti/grlib/lib/ccs/m4/grlib.a \
    -lthird_party/spiffs/lib/ccs/m4/spiffs.a \
    -lti/drivers/lib/drivers_cc32xx.aem4 \
    -lthird_party/fatfs/lib/ccs/m4/fatfs.a \
    "-i$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/source/ti/drivers/net/wifi" \
    -lccs/rtos/simplelink.a \
    "-i$(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)/kernel/tirtos/packages" \
    -lti/dpl/lib/dpl_cc32xx.aem4 \
    "-l$(KERNEL_BUILD)/ccs/linker.cmd" \
    -lti/devices/cc32xx/driverlib/ccs/Release/driverlib.a \
    ../../tirtos/ccs/CC3220SF_LAUNCHXL_TIRTOS.cmd \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --diag_wrap=off \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

$(CONFIGPKG)/linker.cmd $(CONFIGPKG)/compiler.opt:
	@ $(ECHOBLANKLINE)
	@ echo $(CONFIGPKG) is not built.
	@ echo You can build it by issuing $(MAKE) in $(CONFIGPKG).
	@ $(ECHOBLANKLINE)
	
main_tirtos.obj: ../../tirtos/main_tirtos.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

CC3220SF_LAUNCHXL.obj: ../../cc3220launch/CC3220SF_LAUNCHXL.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
main_thread.obj: ../../main_thread.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
uart_terminal.obj: ../../uart_terminal.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

tcp.obj: ../../app/tcp.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

system.obj: ../../app/system.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
shelltask.obj: ../../shelltask.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
		
network_handler.obj: ../../app/network_handler.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
		
sflash.obj: ../../app/sflash.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
userconfig.obj: ../../app/userconfig.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
#tftp.obj: ../../ftp/tftp.c $(CONFIGPKG)/compiler.opt
#	@ echo Building $@
#	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
osi_posix.obj: ../../osi_posix.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
xmlcreate.obj: ../../app/xmlcreate.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
apconfig.obj: ../../app/apconfig.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
cc3220_system.obj: ../../app/cc3220_system.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
nxcGenUDID_24.obj: ../../app/nxcGenUDID_24.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@
	
#spi_handler.obj: ../../spi_handler.c $(CONFIGPKG)/compiler.opt
#	@ echo Building $@
#	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

spi_handler_temp.obj: ../../spi_handler_temp.c $(CONFIGPKG)/compiler.opt	
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@


$(NAME).out: $(OBJECTS) $(CONFIGPKG)/linker.cmd
	@ echo linking...
	@ $(LNK) $(CFLAGS) -z $(OBJECTS) $(LFLAGS) -o $(NAME).out
	@ echo $(NAME).out

clean:
	@ echo Cleaning...
	@ echo $(SIMPLELINK_CC32XX_SDK_INSTALL_DIR)
	@ echo $(abspath ../../)
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
