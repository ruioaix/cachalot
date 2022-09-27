CURRENT_DIR = $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
DIRS = print
include $(addsuffix /Makefile,$(addprefix $(CURRENT_DIR)/,$(DIRS)))
