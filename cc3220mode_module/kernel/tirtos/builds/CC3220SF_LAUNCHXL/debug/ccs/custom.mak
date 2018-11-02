## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em4 linker.cmd package/cfg/debug_pem4.oem4

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/debug_pem4.xdl
	$(SED) 's"^\"\(package/cfg/debug_pem4cfg.cmd\)\"$""\"/media/sf_CC3220/cc3220mod/cc3220mode_module/kernel/tirtos/builds/CC3220SF_LAUNCHXL/debug/ccs/\1\""' package/cfg/debug_pem4.xdl > $@
	-$(SETDATE) -r:max package/cfg/debug_pem4.h compiler.opt compiler.opt.defs
