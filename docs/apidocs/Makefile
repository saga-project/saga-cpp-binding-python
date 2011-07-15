#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
#  Copyright (c) 2005-2010 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_PYTHON_ROOT = ../../

# FIXME: those should come form configure
SAGA_PY_VERSION     = $(shell grep 'VERSION:' $(SAGA_PYTHON_ROOT)/PACKAGE | cut -f 2 -d ' ')
EPYDOC_BIN       = $(shell which epydoc)

# keep empty default target
all::

-include $(SAGA_PYTHON_ROOT)/config/make.cfg
-include $(SAGA_PYTHON_ROOT)/config/saga.config.python.c.mk
-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk


.PHONY: apidoc python
apidoc:: python

python:
ifeq "x$(EPYDOC_BIN)" "x"
	@$(ECHO) "      Cannot generate Python API docs - epydoc is not installed"
else
	@$(ECHO) "      generating C++ API docs"
	@if ! test -d html/ ; \
	then $(MKDIR) 	html ; fi
#	@$(CP) 		 saga-api.png html
	@sed "s/###PROJECT_VERSION###/$(SAGA_PY_VERSION)/g" epydoc.cfg.in > epydoc.cfg
	$(EPYDOC_BIN) --html --config=epydoc.cfg

#	@if test -d python; then echo "python subdir already exists - skip doc generation"; \
#  else \
#  $(ECHO) "      generating Python API docs (saga modules must be in PYTHONPATH)"; \
#  $(EPYDOC_BIN) --html --config=epydoc.cfg.in; \
#  fi
endif

#	$(ECHO) $(SAGA_LOCATION) ;\
#	sed "s/###PROJECT_NUMBER###/$(SAGA_PY_VERSION)/g" epydoc.cfg.in > epydoc.cfg


.PHONY: install
install::
	@if test -d html/ ; \
	then $(ECHO) "      installing Python API documentation"; \
	$(INSTALL) -d html share/saga/doc/python/; \
	else $(ECHO) "      Cannot find Python API documentation - call 'make apidoc' to generate it"; fi
	

distclean::
	@$(RM) epydoc.cfg
