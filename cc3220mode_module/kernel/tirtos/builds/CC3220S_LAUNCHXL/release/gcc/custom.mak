## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,m4g linker.cmd package/cfg/release_pm4g.om4g

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/release_pm4g.xdl
	$(SED) 's"^\"\(package/cfg/release_pm4gcfg.cmd\)\"$""\"/media/sf_CC3220/cc3220mod/cc3220mode_module/kernel/tirtos/builds/CC3220S_LAUNCHXL/release/gcc/\1\""' package/cfg/release_pm4g.xdl > $@
	-$(SETDATE) -r:max package/cfg/release_pm4g.h compiler.opt compiler.opt.defs
