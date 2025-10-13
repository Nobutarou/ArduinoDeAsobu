################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/smc_gen/r_bsp/mcu/all/r_bsp_common_llvm.asm 

S_UPPER_SRCS += \
../src/smc_gen/r_bsp/mcu/all/start.S 

C_SRCS += \
../src/smc_gen/r_bsp/mcu/all/r_bsp_common.c 

CREF += \
LCD_Hello.cref 

C_DEPS += \
./src/smc_gen/r_bsp/mcu/all/r_bsp_common.d 

OBJS += \
./src/smc_gen/r_bsp/mcu/all/r_bsp_common.o \
./src/smc_gen/r_bsp/mcu/all/r_bsp_common_llvm.o \
./src/smc_gen/r_bsp/mcu/all/start.o 

ASM_DEPS += \
./src/smc_gen/r_bsp/mcu/all/r_bsp_common_llvm.d 

MAP += \
LCD_Hello.map 

S_UPPER_DEPS += \
./src/smc_gen/r_bsp/mcu/all/start.d 


# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_bsp/mcu/all/%.o: ../src/smc_gen/r_bsp/mcu/all/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-Og -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -g -mcpu=s3 -std=gnu99 -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/all" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/general" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_pincfg" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24/register_access/llvm" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_config" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/board/generic_rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_IICA0" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_UART1" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" "$<" -c -o "$@")
	@clang --target=rl78 @"$@.in"
src/smc_gen/r_bsp/mcu/all/%.o: ../src/smc_gen/r_bsp/mcu/all/%.asm
	@echo 'Building file: $<'
	$(file > $@.in,-Og -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -g -mcpu=s3 -x assembler-with-cpp -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/all" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/general" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_pincfg" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24/register_access/llvm" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_config" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/board/generic_rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_IICA0" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_UART1" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c "$<" -o "$@")
	@clang --target=rl78 @"$@.in"
src/smc_gen/r_bsp/mcu/all/%.o: ../src/smc_gen/r_bsp/mcu/all/%.S
	@echo 'Building file: $<'
	$(file > $@.in,-Og -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -g -mcpu=s3 -x assembler-with-cpp -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/all" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/general" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_pincfg" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/mcu/rl78_g24/register_access/llvm" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_config" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/r_bsp/board/generic_rl78_g24" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_IICA0" -I"/home/snob/Documents/e2studio/LCD_Hello/src/smc_gen/Config_UART1" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c "$<" -o "$@")
	@clang --target=rl78 @"$@.in"

