#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /media/sf_CC3220/cc3220mod/cc3220mode_module/source;/media/sf_CC3220/cc3220mod/cc3220mode_module/kernel/tirtos/packages
override XDCROOT = /home/heo/ti/xdctools_3_50_08_24_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /media/sf_CC3220/cc3220mod/cc3220mode_module/source;/media/sf_CC3220/cc3220mod/cc3220mode_module/kernel/tirtos/packages;/home/heo/ti/xdctools_3_50_08_24_core/packages;..
HOSTOS = Linux
endif
