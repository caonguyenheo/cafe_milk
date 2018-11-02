#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.M4{1,0,18.1,3
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4: package/package_ti.net.mqtt.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/package -fr=./package/lib/lib/ccs/m4/mqtt_release/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/package -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/package -fr=./package/lib/lib/ccs/m4/mqtt_release/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.sem4: package/package_ti.net.mqtt.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/package -fr=./package/lib/lib/ccs/m4/mqtt_release/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/package -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/package -fr=./package/lib/lib/ccs/m4/mqtt_release/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4: interface/mqttclient.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/interface -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.sem4: interface/mqttclient.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/interface -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4: interface/mqttserver.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/interface -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.sem4: interface/mqttserver.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/interface -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/interface -fr=./package/lib/lib/ccs/m4/mqtt_release/interface
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4: common/mqtt_common.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/common -fr=./package/lib/lib/ccs/m4/mqtt_release/common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/common -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/common -fr=./package/lib/lib/ccs/m4/mqtt_release/common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.sem4: common/mqtt_common.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/common -fr=./package/lib/lib/ccs/m4/mqtt_release/common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/common -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/common -fr=./package/lib/lib/ccs/m4/mqtt_release/common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4: platform/mqtt_net_func.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/platform -fr=./package/lib/lib/ccs/m4/mqtt_release/platform -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/platform -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/platform -fr=./package/lib/lib/ccs/m4/mqtt_release/platform
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.sem4: platform/mqtt_net_func.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/platform -fr=./package/lib/lib/ccs/m4/mqtt_release/platform -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/platform -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/platform -fr=./package/lib/lib/ccs/m4/mqtt_release/platform
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4: client/client_core.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/client -fr=./package/lib/lib/ccs/m4/mqtt_release/client -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/client -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/client -fr=./package/lib/lib/ccs/m4/mqtt_release/client
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/client/client_core.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/client/client_core.sem4: client/client_core.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/client -fr=./package/lib/lib/ccs/m4/mqtt_release/client -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/client -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/client -fr=./package/lib/lib/ccs/m4/mqtt_release/client
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/client/client_core.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/client/client_core.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4: server/client_mgmt.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.sem4: server/client_mgmt.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4: server/server_core.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/server/server_core.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_core.sem4: server/server_core.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_core.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_core.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4: server/server_pkts.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.sem4: server/server_pkts.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4: server/server_plug.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/server/server_plug.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.sem4: server/server_plug.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_plug.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4.dep
package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4.dep: ;
endif

package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4: server/server_util.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/mqtt_release/server/server_util.sem4: | .interfaces
package/lib/lib/ccs/m4/mqtt_release/server/server_util.sem4: server/server_util.c lib/ccs/m4/mqtt_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/mqtt_release/server -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/mqtt_release/server -fr=./package/lib/lib/ccs/m4/mqtt_release/server
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/mqtt_release/server/server_util.sem4: export C_DIR=
package/lib/lib/ccs/m4/mqtt_release/server/server_util.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

clean,em4 ::
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/client/client_core.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_core.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_plug.sem4
	-$(RM) package/lib/lib/ccs/m4/mqtt_release/server/server_util.sem4

lib/ccs/m4/mqtt_release.a: package/lib/lib/ccs/m4/mqtt_release/package/package_ti.net.mqtt.oem4 package/lib/lib/ccs/m4/mqtt_release/interface/mqttclient.oem4 package/lib/lib/ccs/m4/mqtt_release/interface/mqttserver.oem4 package/lib/lib/ccs/m4/mqtt_release/common/mqtt_common.oem4 package/lib/lib/ccs/m4/mqtt_release/platform/mqtt_net_func.oem4 package/lib/lib/ccs/m4/mqtt_release/client/client_core.oem4 package/lib/lib/ccs/m4/mqtt_release/server/client_mgmt.oem4 package/lib/lib/ccs/m4/mqtt_release/server/server_core.oem4 package/lib/lib/ccs/m4/mqtt_release/server/server_pkts.oem4 package/lib/lib/ccs/m4/mqtt_release/server/server_plug.oem4 package/lib/lib/ccs/m4/mqtt_release/server/server_util.oem4 lib/ccs/m4/mqtt_release.a.mak

clean::
	-$(RM) lib/ccs/m4/mqtt_release.a.mak
