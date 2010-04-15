#  Copyright (c) 2010 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_PYTHON_ROOT = ../

# keep empty default target
all::

-include $(SAGA_PYTHON_ROOT)/config/make.cfg
-include $(SAGA_PYTHON_ROOT)/config/saga.config.python.c.mk

.PHONY: apidoc 
apidoc:
	@$(MAKE) -C apidocs apidoc

.PHONY: install
install:
	@$(MAKE) -C apidocs install

