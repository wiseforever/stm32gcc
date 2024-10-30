# app.mk

# CSRCS += $(wildcard $(APP_DIR)/$(APP_DIR_NAME)/*.c)
# CXXSRCS += $(wildcard $(APP_DIR)/$(APP_DIR_NAME)/*.cpp)
CSRCS += app_main.c
CXXSRCS += 

DEPPATH += --dep-path $(APP_DIR)/$(APP_DIR_NAME)
VPATH += $(APP_DIR)/$(APP_DIR_NAME)

CFLAGS += -I$(APP_DIR)/$(APP_DIR_NAME)

include $(APP_DIR)/$(APP_DIR_NAME)/custom/custom.mk
# include $(APP_DIR)/$(APP_DIR_NAME)/systask/systask.mk


# $(info APP_DIR = $(APP_DIR)/$(APP_DIR_NAME))

