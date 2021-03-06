# $Id: config.mk 657 2008-02-21 17:37:34Z lsc $
# 
# Copyright (c) 2008 STFC Rutherford Appleton Laboratory 
# Author: Lee-Shawn Chin 
# Date  : Feb 2008
#
#
# ----------- Module configuration ------------
#
# included by ../../Makefile

# Name of this module
MODULE_NAME = utils

# Modules this module depend on
MODULE_DEPENDS = 

# Additional header files used, apart from mb_$(MODULE_NAME).h
EXTRA_INC = mboard.h mb_common.h uthash.h

# Load common include.mk to do the rest
include include.mk
