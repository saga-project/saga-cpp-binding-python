#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_SRC               = $(wildcard *.cpp)
SAGA_PY_MODULE_OBJ     = $(SAGA_SRC:%.cpp=%.o)
SAGA_PY_MODULE         = _advert.so
SAGA_PY_MODULE_DIR     = advert/
SAGA_REQUIRE_PACKAGES  = advert

-include ../../config/make.cfg
-include ../../config/saga.python.mk

check::
	@$(MKDIR) ../../tmp_install/saga/advert/
	@$(CP) $(SAGA_PY_MODULE) __init__.py ../../tmp_install/saga/advert/

