BINARYNAME = main

OPTFLAG = -O3

LOADADDR := 0xC2000040
ENTRYPOINT := $(LOADADDR)

LINKSCR = linker/stm32mp15xx_ca7.ld

SOURCES =  src/main.cc
SOURCES += src/mp153-drivers/mmu_ca7.c
SOURCES += src/mp153-drivers/aux_core_main.cc
SOURCES += src/console.cc
SOURCES += src/calibration_storage.cc
SOURCES += src/libc_stub.c
SOURCES += src/libcpp_stub.cc

SOURCES += lib/printf/printf.c

SOURCES += lib/mdrivlib/drivers/pin.cc
SOURCES += lib/mdrivlib/drivers/tim.cc
SOURCES += lib/mdrivlib/drivers/timekeeper.cc
SOURCES += lib/mdrivlib/drivers/i2c.cc
SOURCES += lib/mdrivlib/drivers/codec_PCM3060.cc
SOURCES += lib/mdrivlib/target/stm32mp1_ca7/boot/irq_ctrl.c
SOURCES += lib/mdrivlib/target/stm32mp1/drivers/sai_tdm.cc
SOURCES += lib/mdrivlib/target/stm32mp1_ca7/drivers/interrupt_handler.cc
SOURCES += lib/mdrivlib/target/stm32mp1_ca7/boot/startup.s
SOURCES += lib/mdrivlib/target/stm32mp1_ca7/boot/system_init.c

SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_adc.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_adc_ex.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_cortex.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_dac.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_dma.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_exti.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_gpio.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_i2c.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_i2c_ex.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_pwr.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_pwr_ex.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_rcc.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_rcc_ex.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_sai.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_tim.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_uart.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_usart.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_hal_sd.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_ll_tim.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_ll_fmc.c
SOURCES += lib/STM32MP1xx_HAL_Driver/Src/stm32mp1xx_ll_sdmmc.c

SOURCES += lib/libhwtests/src/AdcChecker.cc
SOURCES += lib/libhwtests/src/AdcRangeChecker.cc
SOURCES += lib/libhwtests/src/ButtonChecker.cc
SOURCES += lib/libhwtests/src/CodecCallbacks.cc
SOURCES += lib/libhwtests/src/GateInChecker.cc
SOURCES += lib/libhwtests/src/GateOutChecker.cc
SOURCES += lib/libhwtests/src/GateOutput.cc
SOURCES += lib/libhwtests/src/LEDTester.cc

INCLUDES  = -I. 
INCLUDES += -Isrc 
INCLUDES += -Isrc/hardware_tests 
INCLUDES += -Isrc/mp153-drivers 
INCLUDES += -Ilib 
INCLUDES += -Ilib/brainboard 
INCLUDES += -Ilib/brainboard/mp153 
INCLUDES += -Ilib/libhwtests/inc
INCLUDES += -Ilib/mdrivlib 
INCLUDES += -Ilib/mdrivlib/target/stm32mp1 
INCLUDES += -Ilib/mdrivlib/target/stm32mp1_ca7 
INCLUDES += -Ilib/cpputil 
INCLUDES += -Ilib/CMSIS/Include 
INCLUDES += -Ilib/CMSIS/Core_A/Include 
INCLUDES += -Ilib/CMSIS/Device/ST/STM32MP1xx/Include 
INCLUDES += -Ilib/STM32MP1xx_HAL_Driver/Inc 

MCU = -mcpu=cortex-a7 -march=armv7ve -mfpu=neon-vfpv4 -mlittle-endian -mfloat-abi=hard

ARCH_CFLAGS = -DUSE_FULL_LL_DRIVER \
			  -DUSE_HAL_DRIVER \
			  -DSTM32MP157Cxx \
			  -DSTM32MP1 \
			  -DCORE_CA7 \
			  $(EXTRA_ARCH_CFLAGS) \

AFLAGS = $(MCU)

CFLAGS = -g2 \
		 -fno-common \
		 $(ARCH_CFLAGS) \
		 $(MCU) \
		 $(INCLUDES) \
		 -fdata-sections -ffunction-sections \
		 -nostartfiles \
		 -ffreestanding \
  	     -mtune=cortex-a7 \
		 -mvectorize-with-neon-quad \
		 -funsafe-math-optimizations \
		 $(EXTRACFLAGS)\

CXXFLAGS = $(CFLAGS) \
		-std=c++2b \
	    -ffold-simple-inlines \
		-fno-rtti \
		-fno-exceptions \
		-fno-unwind-tables \
		-ffreestanding \
		-fno-threadsafe-statics \
		-mno-unaligned-access \
		-Werror=return-type \
		-Wdouble-promotion \
		-Wno-register \
		-Wno-volatile \
		 $(EXTRACXXFLAGS) \

LFLAGS = -Wl,--gc-sections \
		 -Wl,-Map,$(BUILDDIR)/$(BINARYNAME).map,--cref \
		 $(MCU)  \
		 -T $(LINKSCR) \
		 -nostdlib \
		 -nostartfiles \
		 -ffreestanding \
		 $(EXTRALDFLAGS) \

DEPFLAGS = -MMD -MP -MF $(OBJDIR)/$(basename $<).d

ARCH 	= arm-none-eabi
CC 		= $(ARCH)-gcc
CXX 	= $(ARCH)-g++
LD 		= $(ARCH)-g++
AS 		= $(ARCH)-as
OBJCPY 	= $(ARCH)-objcopy
OBJDMP 	= $(ARCH)-objdump
GDB 	= $(ARCH)-gdb
SZ 		= $(ARCH)-size

SZOPTS 	= -d

ELF 	= $(BUILDDIR)/$(BINARYNAME).elf
HEX 	= $(BUILDDIR)/$(BINARYNAME).hex
BIN 	= $(BUILDDIR)/$(BINARYNAME).bin

OBJECTS   = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SOURCES))))
DEPS   	  = $(addprefix $(OBJDIR)/, $(addsuffix .d, $(basename $(SOURCES))))
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj/obj

all: image
all: Makefile $(ELF) 

$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(AS) $(AFLAGS) $< -o $@ 

$(OBJDIR)/%.o: %.c $(OBJDIR)/%.d
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(CC) -c $(DEPFLAGS) $(OPTFLAG) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c[cp]* $(OBJDIR)/%.d
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(CXX) -c $(DEPFLAGS) $(OPTFLAG) $(CXXFLAGS) $< -o $@

$(ELF): $(OBJECTS) $(LINKSCR) Makefile
	$(info Linking...)
	@$(LD) $(LFLAGS) -o $@ $(OBJECTS) 

$(BIN): $(ELF)
	$(OBJCPY) -O binary $< $@

$(HEX): $(ELF)
	@$(OBJCPY) --output-target=ihex $< $@
	@$(SZ) $(SZOPTS) $(ELF)

image: $(BUILDDIR)/$(BINARYNAME).uimg 

$(BUILDDIR)/$(BINARYNAME).uimg: $(BIN)
	python3 uimg_header.py $(BUILDDIR)/$(BINARYNAME).bin $(BUILDDIR)/$(BINARYNAME).uimg $(LOADADDR) $(ENTRYPOINT)
	@ls -l $(BUILDDIR)/$(BINARYNAME).*

flash: $(BUILDDIR)/$(BINARYNAME).uimg
	$(info ------------------------------------------------------------------------)
	$(info FLASHING)
	$(info Reboot the board with the jumper installed before executing this command)
	$(info ------------------------------------------------------------------------)
	dfu-util -a 0 -s 0x70080000 -D build/mp153/RelWithDebInfo/mp153.uimg

bootloader:
	cd bootloader/mp1-boot && make clean && make BOARD_CONF=../brainboard_p3_conf.hh

compile_commands:
	compiledb make
	compdb -p ./ list > compile_commands.tmp 2>/dev/null
	rm compile_commands.json
	mv compile_commands.tmp compile_commands.json

%.d: ;

clean:
	rm -rf $(BUILDDIR)

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif

.PRECIOUS: $(DEPS) $(OBJECTS) $(ELF)
.PHONY: all clean compile_commands bootloader flash
