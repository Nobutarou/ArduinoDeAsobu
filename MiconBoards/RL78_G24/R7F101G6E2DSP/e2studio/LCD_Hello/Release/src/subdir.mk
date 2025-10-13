################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_SCRIPT += \
../src/linker_script.ld 

C_SRCS += \
../src/LCD_Hello.c 

CREF += \
LCD_Hello.cref 

C_DEPS += \
./src/LCD_Hello.d 

OBJS += \
./src/LCD_Hello.o 

MAP += \
LCD_Hello.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-Og -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -g -mcpu=s3 -std=gnu99 -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/all" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/general" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_pincfg" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24/register_access/llvm" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_config" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/board/generic_rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_IICA0" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_UART1" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" "$<" -c -o "$@")
	@clang --target=rl78 @"$@.in"

