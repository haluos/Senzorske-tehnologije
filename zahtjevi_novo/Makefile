include ~/Desktop/makefile/project.mk
# # Put your stlink folder here so make burn will work.
# STLINK=~/stlink.git
#
# # Put your source files here (or *.c, etc)
# SRCS=main.c dataOrganization.c \
# usart.c gpio.c timer.c packing.c \
# system_stm32f4xx.c misc.c \
# stm32f4xx_usart.c stm32f4xx_rcc.c \
# stm32f4xx_gpio.c stm32f4xx_dma.c \
# stm32f4xx_tim.c
# INC=main.h dataOrganization.h \
# usart.h gpio.h timer.h packing.h \
# system_stm32f4xx.h stm32f4xx_usart.h stm32f4xx_rcc.h \
# stm32f4xx_gpio.h stm32f4xx_dma.h \
# stm32f4xx_tim.h
#
# # Binaries will be generated with this name (.elf, .bin, .hex, etc)
# PROJ_NAME=zahtjevi
#
# # Put your STM32F4 library code directory here
# STM_COMMON=../STM32F4-Discovery_FW_V1.1.0
#
# #Path to objects directory
# ODIR = ./objects
#
# # Normally you shouldn't need to change anything below this line!
# #######################################################################################
#
# CC=arm-none-eabi-gcc
# LD = arm-none-eabi-ld
# OBJCOPY=arm-none-eabi-objcopy
#
# CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld
# # CFLAGS = -Wall -O0 -g
# CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
# # CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# CFLAGS += -msoft-float -mfpu=fpv4-sp-d16
# CFLAGS += -I.
#
# # Include files from STM libraries
# CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
# CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
# CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
#
# # add startup file to build
# SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s
# # STARTUP = $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s
# OBJS = $(SRCS:.c=.o)
# # OBJS = $(patsubst %,$(ODIR)/%,$($(SRCS:.c=.o)))
# # OBJS += $(INC:.h=.o)
#
# CPFLAGS = -O binary
# LFLAGS = -Tstm32_flash.ld -nostartfiles
#
# .PHONY: proj clean burn
#
# all: proj
#
# # proj: $(PROJ_NAME).elf
# proj: $(PROJ_NAME).bin
#
# $(PROJ_NAME).bin: $(PROJ_NAME).elf
# 	# $(OBJCOPY) $(CPFLAGS) $< $@
#
# # $(PROJ_NAME).elf: $(SRCS)
# # 	$(CC) $(CFLAGS) $^ -o $@
# # 	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
# # 	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
#
# $(PROJ_NAME).elf: $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) -o $@
# 	# $(CC) $(CFLAGS) -c $(SRCS)
# 	# $(CC) $(CFLAGS) -o $@ $^ $(STARTUP)
# 	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
# 	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
#
# $(OBJS) : defines.h
# main.o : main.c main.h dataOrganization.h dataOrganization.c
# 	$(CC) $(CFLAGS) -c main.c
# packing.o : packing.c packing.h defines.h
# 	$(CC) $(CFLAGS) -c packing.c
# dataOrganization.o : dataOrganization.c dataOrganization.h defines.h \
# usart.c usart.h packing.c packing.h timer.c timer.h
# 	$(CC) $(CFLAGS) -c dataOrganization.c
# usart.o : usart.c usart.h defines.h timer.c timer.h gpio.c gpio.h
# 	$(CC) $(CFLAGS) -c usart.c
# gpio.o : gpio.c gpio.h defines.h
# 	$(CC) $(CFLAGS) -c gpio.c
# timer.o : timer.c timer.h defines.h
# 	$(CC) $(CFLAGS) -c timer.c
#
# # %.o: %.c
# 	# $(CC) $(CFLAGS) $^ -c $@ $(STARTUP)
#
# clean:
# 	rm -f *.o $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin
#
# # Flash the STM32F4
# burn: proj
# 	$(STLINK)/build/Release/st-flash write $(PROJ_NAME).bin 0x8000000
