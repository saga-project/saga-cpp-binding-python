#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)


-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk
-include config/make.cfg
-include config/saga.config.python.c.mk

ifeq "$(SAGA_HAVE_PYTHON)-$(SAGA_HAVE_BOOST_PYTHON)" "yes-yes"
  SAGA_SUBDIRS = config external engine packages
endif

-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk
-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.dist.mk

# directory dependencies
engine:   external
packages: external

