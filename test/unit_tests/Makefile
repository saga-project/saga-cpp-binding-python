#
#   Copyright (c) 2008 João Abecasis
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

include ../../config/make.cfg
include ../../config/saga.config.python.c.mk

SAGA_DONT_INSTALL = yes
SCRIPT            = saga_python_tests.py

include $(SAGA_MAKE_INCLUDE_ROOT)/saga.test.mk

check:: $(SCRIPT).run
$(SCRIPT).run: $(SCRIPT)
	@$(ECHO)  "===================================="; \
   $(ECHO)  "=== TEST COMMAND ==================="; \
   $(ECHO)  "===================================="; \
   $(ECHO)  "$<"; \
   $(ECHO)  "";   \
   $(ECHO)  "===================================="; \
   $(ECHO)  "=== TEST OUTPUT  ==================="; \
   $(ECHO)  "===================================="; \
   $(RM)    "$@.nok"; \
   $(ENV) $(SAGA_ENVIRON) PYTHONPATH=`$(PWD)`/../../tmp_install/ $(SAGA_PYTHON) ./$< \
            && $(TOUCH) $@ \
            || $(TOUCH) $@.nok; \
   $(ECHO)  ""; \
   $(ECHO)  "===================================="; \
   $(ECHO)  "=== TEST_RESULT  ==================="; \
   $(ECHO)  "===================================="; \
   $(TEST) -e $@ && $(ECHO)  "SUCCESS"; \
   $(TEST) -e $@ || $(ECHO)  "FAILURE"; \
   $(ECHO)  ""; \
   $(ECHO)  "===================================="; \
   test -e $@.nok && $(FALSE); \
   test -e $@     && $(TRUE);

clean::
	@$(RM) $(SCRIPT).run

checkclean:: clean

