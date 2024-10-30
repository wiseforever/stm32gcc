# config.mk

### compiler version
C_VERNUM := c11
CXX_VERNUM := c++11
C_VER := -std=$(C_VERNUM)
CXX_VER := -std=$(CXX_VERNUM)


### c and C ++ some defines
CDEFS += 


# ### 
# SRC_ROOT_DIRS :=  bsp
# INC_ROOT_DIRS := $(SRC_ROOT_DIRS)


# ### include mk
# APP_DIR := .
# APP_DIR_NAME := app
# LIB_DIR := .
# LIB_DIR_NAME := lib
# include $(APP_DIR)/$(APP_DIR_NAME)/app.mk
# include $(LIB_DIR)/$(LIB_DIR_NAME)/lib.mk


### linker script
LINKER_SCRIPT := 
LDSCRIPT += $(foreach dir, $(LINKER_SCRIPT), -T$(dir))

### sdram linker script
LINKER_SCRIPT_FILE := STM32F429IGTx_FLASH.ld
SZ_OPTION := -G

# SDRAM_LINKER_SCRIPT := sdram.ld
ifneq ($(SDRAM_LINKER_SCRIPT), )
LDSCRIPT += $(foreach dir, $(SDRAM_LINKER_SCRIPT), -T$(dir))
SDRAM_M := $(shell grep -E '^\s*SDRAM \(xrw\)' $(SDRAM_LINKER_SCRIPT) | awk -F 'LENGTH = ' '{print $$2}' | awk '{print $$1}' | sed 's/M//')
SDRAM_BYTES := $(shell echo "$$(( $(SDRAM_M) * 1024 * 1024 ))")
endif


### linker lib
# -lrdimon: The lrdimon library provides an easy way to implement printf
# -u_printf_float: Support for float printing
LIBS += -lrdimon
# LIBDIR += -Lpath/to/lib


### c and C ++ some parameter configuration
CFLAGS += $(CDEFS)
# Treat warnings about "implicit function declarations" as errors.
CFLAGS += -Werror=implicit-function-declaration
# Disable warnings for multi-line comments
CFLAGS += -Wno-comment
# Disable warnings for unused functions
CFLAGS += -Wno-unused-function
# Disable warnings for unused variables
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-but-set-variable
# # Disable warnings for uninitialized variables
# CFLAGS += -Wno-maybe-uninitialized
# # Ignore warnings related to string operation overflows
# CFLAGS += -Wno-stringop-overflow

# 通过 filter-out 去除 CFLAGS 中的 -Werror=implicit-function-declaration
FLAGS_filter_out := -Werror=implicit-function-declaration
CXXFLAGS_filter_out += $(filter-out $(FLAGS_filter_out), $(CFLAGS))


### 
ifeq ($(c_link_cpp), 1)
LIBS += -lstdc++
endif







### Define Colors
COLOR_RESET   = \x1b[0m
COLOR_GREEN   = \x1b[32m  # C 文件编译成功
COLOR_BLUE    = \x1b[34m  # C++ 文件编译成功
COLOR_PURPLE  = \x1b[35m  # 链接操作
COLOR_YELLOW  = \x1b[33m  # 警告
COLOR_RED     = \x1b[31m  # 错误

### Compile Rule
# compile_show_type : NONE COUNTER or PER
compile_show_type := NONE
ifeq ($(compile_show_type), NONE)

define compile_rule
	@echo -e "$(COLOR_GREEN)\rCompiling $< $(COLOR_RESET)"
endef
define cpp_compile_rule
	@echo -e "$(COLOR_BLUE)\rCpp Compiling $< $(COLOR_RESET)"
endef
define as_compile_rule
	@echo -e "$(COLOR_PURPLE)\rAs Compiling $< $(COLOR_RESET)"
endef

else ifeq ($(compile_show_type), COUNTER)

# define compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	@echo -e "$(COLOR_GREEN)[${COUNTER}/$(TOTAL_FILES)]\rCompiling $< $(COLOR_RESET)"
# endef
# define cpp_compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	@echo -e "$(COLOR_BLUE)[${COUNTER}/$(TOTAL_FILES)]\rCpp Compiling $< $(COLOR_RESET)"
# endef
# define as_compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	@echo -e "$(COLOR_PURPLE)[${COUNTER}/$(TOTAL_FILES)]\rAs Compiling $< $(COLOR_RESET)"
# endef

# else ifeq ($(compile_show_type), PER)

# define compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	$(eval PER=$(shell echo $$(($(COUNTER) * 100 / $(TOTAL_FILES)))))
# 	@echo -e "$(COLOR_GREEN)\r[${PER}%]Compiling $< $(COLOR_RESET)"
# endef
# define cpp_compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	$(eval PER=$(shell echo $$(($(COUNTER) * 100 / $(TOTAL_FILES)))))
# 	@echo -e "$(COLOR_BLUE)\r[${PER}%]Cpp Compiling $< $(COLOR_RESET)"
# endef
# define as_compile_rule
# 	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
# 	$(eval PER=$(shell echo $$(($(COUNTER) * 100 / $(TOTAL_FILES)))))
# 	@echo -e "$(COLOR_PURPLE)\r[${PER}%]]As Compiling $< $(COLOR_RESET)"
# endef

endif

### 
define linking_elf_s
	@echo 
	@echo -e "$(COLOR_PURPLE)\rlinking $@ $(COLOR_RESET)"
	@echo -e "$(COLOR_GREEN)\r########## ########## ########## ########## ########## ########## ########## ########## "
endef
define linking_elf_e
	@echo -e "########## ########## ########## ########## ########## ########## ########## ########## "
endef

###
# 提取 RAM CCMRAM FLASH SDRAM 的容量，去掉单位 K M
RAM_KB := $(shell grep -E '^\s*RAM \(xrw\)' $(LINKER_SCRIPT_FILE) | awk -F 'LENGTH = ' '{print $$2}' | awk '{print $$1}' | sed 's/K//')
CCMRAM_KB := $(shell grep -E '^\s*CCMRAM \(xrw\)' $(LINKER_SCRIPT_FILE) | awk -F 'LENGTH = ' '{print $$2}' | awk '{print $$1}' | sed 's/K//')
FLASH_KB := $(shell grep -E '^\s*FLASH \(rx\)' $(LINKER_SCRIPT_FILE) | awk -F 'LENGTH = ' '{print $$2}' | awk '{print $$1}' | sed 's/K//')

# 
RAM_BYTES := $(shell echo "$$(( $(RAM_KB) * 1024 ))")
CCMRAM_BYTES := $(shell echo "$$(( $(CCMRAM_KB) * 1024 ))")
FLASH_BYTES := $(shell echo "$$(( $(FLASH_KB) * 1024 ))")

ifneq ($(SDRAM_LINKER_SCRIPT), )
SRAM_TOTAL := $(shell echo "$$(( $(RAM_BYTES) + $(CCMRAM_BYTES) + $(SDRAM_BYTES) ))")
else
SRAM_TOTAL := $(shell echo "$$(( $(RAM_BYTES) + $(CCMRAM_BYTES) ))")
endif
FLASH_TOTAL := $(FLASH_BYTES)

ifeq ($(SZ_OPTION), -A)
SZ_FORMAT := | awk ' \
            NR==1 {print; next} \
            NR==2 { \
                    printf "%-20s %-15s %-15s %s\n", "section", "size", "addr-dec", "addr-hex"; \
                    next \
                } \
            { \
                if ($$1 != "") { \
                    if ($$1 != "Total") \
                        printf "%-20s %-15s %-15s 0x%08X\n", $$1, $$2, $$3, $$3; \
                    else \
                        printf "%-20s %-15s\n", $$1, $$2; \
                } \
            }'
else ifeq ($(SZ_OPTION), -B)

ifneq ($(LINKER_SCRIPT_FILE), )
SZ_FORMAT := | awk ' \
            BEGIN { \
                printf "%-12s %-12s %-12s %-12s %-12s %s\n", "text", "data", "bss", "dec", "hex", "filename"; \
            } \
            NR>1 { \
                if ($$1 != "") { \
                    printf "%-12s %-12s %-12s %-12s %-12s %s\n", $$1, $$2, $$3, $$4, $$5, $$6; \
                } \
            } \
            END { \
                sram = strtonum($$2) + strtonum($$3); \
                flash = strtonum($$1) + strtonum($$2); \
                printf "\nSRAM usage: %d bytes (.data + .bss)\n", sram; \
                printf "Flash usage: %d bytes (.text + .data)\n", flash; \
                sram_percent = (sram * 100) / $(SRAM_TOTAL); \
                flash_percent = (flash * 100) / $(FLASH_TOTAL); \
                printf "SRAM usage: %.2f%% ( %d / %d )\n", sram_percent, sram, $(SRAM_TOTAL); \
                printf "Flash usage: %.2f%% ( %d / %d )\n", flash_percent, flash, $(FLASH_TOTAL); \
            }'
endif

else ifeq ($(SZ_OPTION), -G)

ifneq ($(LINKER_SCRIPT_FILE), )
SZ_FORMAT := | awk ' \
            BEGIN { \
                printf "%-12s %-12s %-12s %-12s %s\n", "text", "data", "bss", "total", "filename"; \
            } \
            NR>1 { \
                if ($$1 != "") { \
                    printf "%-12s %-12s %-12s %-12s %s\n", $$1, $$2, $$3, $$4, $$5; \
                } \
            } \
            END { \
                sram = strtonum($$2) + strtonum($$3); \
                flash = strtonum($$1) + strtonum($$2); \
                printf "\nSRAM usage: %d bytes (.data + .bss)\n", sram; \
                printf "Flash usage: %d bytes (.text + .data)\n", flash; \
                sram_percent = (sram * 100) / $(SRAM_TOTAL); \
                flash_percent = (flash * 100) / $(FLASH_TOTAL); \
                printf "SRAM usage: %.2f%% ( %d / %d )\n", sram_percent, sram, $(SRAM_TOTAL); \
                printf "Flash usage: %.2f%% ( %d / %d )\n", flash_percent, flash, $(FLASH_TOTAL); \
            }'
endif

endif

