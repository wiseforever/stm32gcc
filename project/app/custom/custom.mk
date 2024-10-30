# custom.mk

# CSRCS += $(wildcard $(APP_DIR)/$(APP_DIR_NAME)/custom/*.c)
CSRCS += sdkconfig.c sys.c

DEPPATH += --dep-path $(APP_DIR)/$(APP_DIR_NAME)/custom
VPATH += $(APP_DIR)/$(APP_DIR_NAME)/custom

CFLAGS += -I$(APP_DIR)/$(APP_DIR_NAME)/custom
