#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

-include config/make.cfg
-include config/saga.config.python.c.mk

SAGA_SUBDIRS     = config docs

ifeq "$(SAGA_HAVE_PYTHON)-$(SAGA_HAVE_BOOST_PYTHON)" "yes-yes"

 SAGA_SUBDIRS   += external engine packages 

 ifeq "$(SAGA_BOOST_HAVE_TEST)" "yes"
  ifdef SAGA_IS_CHECKING
   SAGA_SUBDIRS += test
  endif
 endif
endif

all:: config/make.cfg

config/make.cfg: 
	@echo ""
	@echo " ================================= "
	@echo "  you need to run configure first  "
	@echo " ================================= "
	@echo ""
	@false


-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk
-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.dist.mk


distclean::
	@$(FIND) . -name \*.pyc -exec $(RM) {} \;
	@$(RM) config.log
	@$(RM) config.status
	@$(RM) config.summary
	@$(RM) saga-bindings-python.deb.ctrl
	@$(RM) -r tmp_install/


# directory dependencies
engine:   external
packages: external
test:     engine
test:     packages

# support out-of-the-box 'make srcdist'
-include $(SAGA_LOCATION)/share/saga/make/saga.dist.mk
