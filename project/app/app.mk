# app.mk

# CSRCS += $(wildcard $(APP_DIR)/$(APP_DIR_NAME)/*.c)
# CXXSRCS += $(wildcard $(APP_DIR)/$(APP_DIR_NAME)/*.cpp)
# CSRCS += 
# CCSRCS += 
# CXXSRCS += app_main.cpp

# DEPPATH += --dep-path $(APP_DIR)/$(APP_DIR_NAME)
# VPATH += $(APP_DIR)/$(APP_DIR_NAME)

# CFLAGS += -I$(APP_DIR)/$(APP_DIR_NAME)

# include $(APP_DIR)/$(APP_DIR_NAME)/systask/systask.mk
# include $(APP_DIR)/$(APP_DIR_NAME)/custom/custom.mk

CSRCS += $(shell find $(APP_DIR)/$(APP_DIR_NAME) -name "*.c")
CCSRCS += $(shell find $(APP_DIR)/$(APP_DIR_NAME) -name "*.cc")
CXXSRCS += $(shell find $(APP_DIR)/$(APP_DIR_NAME) -name "*.cpp")
CINCS += $(shell find $(APP_DIR)/$(APP_DIR_NAME)  -type d)

DEPPATH +=  $(foreach dir, $(CINCS), --dep-path$(dir))
VPATH += $(CINCS)
CFLAGS += $(foreach dir, $(CINCS), -I$(dir))
