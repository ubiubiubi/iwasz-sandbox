# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/iwasz/workspace/i2c-test/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/iwasz/workspace/i2c-test/build

# Include any dependencies generated for this target.
include CMakeFiles/i2c-test.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/i2c-test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/i2c-test.elf.dir/flags.make

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o: /home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o   -c /home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.i"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c > CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.i

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.s"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c -o CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o: /home/iwasz/workspace/i2c-test/src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o   -c /home/iwasz/workspace/i2c-test/src/main.c

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.i"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/iwasz/workspace/i2c-test/src/main.c > CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.i

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.s"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/iwasz/workspace/i2c-test/src/main.c -o CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o: /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building ASM-ATT object CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/arm-none-eabi/bin/as  $(ASM-ATT_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o: /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o   -c /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.i"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c > CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.i

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.s"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o: /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o   -c /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.i"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c > CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.i

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.s"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o: CMakeFiles/i2c-test.elf.dir/flags.make
CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o: /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/iwasz/workspace/i2c-test/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o   -c /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.i"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c > CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.i

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.s"
	/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c -o CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.s

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.requires:
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.requires

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.provides: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.requires
	$(MAKE) -f CMakeFiles/i2c-test.elf.dir/build.make CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.provides.build
.PHONY : CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.provides

CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.provides.build: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o

# Object files for target i2c-test.elf
i2c__test_elf_OBJECTS = \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o" \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o" \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o" \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o" \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o" \
"CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o"

# External object files for target i2c-test.elf
i2c__test_elf_EXTERNAL_OBJECTS =

i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/build.make
i2c-test.elf: CMakeFiles/i2c-test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable i2c-test.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/i2c-test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/i2c-test.elf.dir/build: i2c-test.elf
.PHONY : CMakeFiles/i2c-test.elf.dir/build

CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/system_stm32f4xx.c.o.requires
CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/workspace/i2c-test/src/main.c.o.requires
CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s.o.requires
CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o.requires
CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o.requires
CMakeFiles/i2c-test.elf.dir/requires: CMakeFiles/i2c-test.elf.dir/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o.requires
.PHONY : CMakeFiles/i2c-test.elf.dir/requires

CMakeFiles/i2c-test.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/i2c-test.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/i2c-test.elf.dir/clean

CMakeFiles/i2c-test.elf.dir/depend:
	cd /home/iwasz/workspace/i2c-test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/iwasz/workspace/i2c-test/build /home/iwasz/workspace/i2c-test/build /home/iwasz/workspace/i2c-test/build /home/iwasz/workspace/i2c-test/build /home/iwasz/workspace/i2c-test/build/CMakeFiles/i2c-test.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/i2c-test.elf.dir/depend

