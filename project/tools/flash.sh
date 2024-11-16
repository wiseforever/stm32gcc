#!/bin/bash

### 使用openocd默认环境时，可以使用以下命令
# openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c "program build/bin/project.elf verify reset exit"

### 当flash.sh文件在项目根目录下时，可以使用以下命令
# openocd -f tools/openocd/interface/flash.cfg -c "program build/bin/project.elf verify reset exit"

### 当flash.sh文件在tools目录下时，可以使用以下命令
openocd -f openocd/interface/flash.cfg -c "program ../build/bin/project.elf verify reset exit"
