#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.M4{1,0,18.1,3
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4.dep
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4.dep: ;
endif

package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4: | .interfaces
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4: package/package_ti.net.http.c lib/ccs/m4/httpclient_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release/package -fr=./package/lib/lib/ccs/m4/httpclient_release/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/httpclient_release/package -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release/package -fr=./package/lib/lib/ccs/m4/httpclient_release/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4: export C_DIR=
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.sem4: | .interfaces
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.sem4: package/package_ti.net.http.c lib/ccs/m4/httpclient_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release/package -fr=./package/lib/lib/ccs/m4/httpclient_release/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/httpclient_release/package -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release/package -fr=./package/lib/lib/ccs/m4/httpclient_release/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.sem4: export C_DIR=
package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4.dep
package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4.dep: ;
endif

package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4: | .interfaces
package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4: httpclient.c lib/ccs/m4/httpclient_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release -fr=./package/lib/lib/ccs/m4/httpclient_release -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/httpclient_release -s oem4 $< -C   -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release -fr=./package/lib/lib/ccs/m4/httpclient_release
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4: export C_DIR=
package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

package/lib/lib/ccs/m4/httpclient_release/httpclient.sem4: | .interfaces
package/lib/lib/ccs/m4/httpclient_release/httpclient.sem4: httpclient.c lib/ccs/m4/httpclient_release.a.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem4 -n $< ...
	$(ti.targets.arm.elf.M4.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release -fr=./package/lib/lib/ccs/m4/httpclient_release -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/ccs/m4/httpclient_release -s oem4 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M4 --float_support=vfplib --abi=eabi -eo.oem4 -ea.sem4  -ms -g --c99   -I/vagrant/SDK_Q3_GA_17_9_18/sdk_root/source/ti/posix/ccs -Dxdc_target_name__=M4 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_18_1_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M4.rootDir)/include  -fs=./package/lib/lib/ccs/m4/httpclient_release -fr=./package/lib/lib/ccs/m4/httpclient_release
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/ccs/m4/httpclient_release/httpclient.sem4: export C_DIR=
package/lib/lib/ccs/m4/httpclient_release/httpclient.sem4: PATH:=$(ti.targets.arm.elf.M4.rootDir)/bin/:$(PATH)

clean,em4 ::
	-$(RM) package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4
	-$(RM) package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4
	-$(RM) package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.sem4
	-$(RM) package/lib/lib/ccs/m4/httpclient_release/httpclient.sem4

lib/ccs/m4/httpclient_release.a: package/lib/lib/ccs/m4/httpclient_release/package/package_ti.net.http.oem4 package/lib/lib/ccs/m4/httpclient_release/httpclient.oem4 lib/ccs/m4/httpclient_release.a.mak

clean::
	-$(RM) lib/ccs/m4/httpclient_release.a.mak
