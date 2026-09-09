################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/smc_gen/r_pincfg/subdir.mk
-include src/smc_gen/r_bsp/mcu/rl78_g24/subdir.mk
-include src/smc_gen/r_bsp/mcu/all/subdir.mk
-include src/smc_gen/r_bsp/board/generic_rl78_g24/subdir.mk
-include src/smc_gen/general/subdir.mk
-include src/smc_gen/Config_UART1/subdir.mk
-include src/smc_gen/Config_IICA0/subdir.mk
-include src/smc_gen/Config_ADC/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
$(C_DEPS):$(SMS_ASM)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \

OBJCOPY += \
LuxMeter1.0.srec \

SECONDARY_SIZE += \
LuxMeter1.0.siz \


# All Target
all: LuxMeter1.0.elf secondary-outputs

# Tool invocations
LuxMeter1.0.elf: $(OBJS) $(USER_OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(LINKER_SCRIPT)
	$(file > LuxMeter1.0.elf.in,-Os -ffunction-sections -fdata-sections -fdiagnostics-parseable-fixits -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -g -mcpu=s3 -o "LuxMeter1.0.elf" $(OBJS) $(USER_OBJS) -T "/home/snob/Documents/e2studio/LuxMeter1.0/src/linker_script.ld" -Wl,-Map=LuxMeter1.0.map -Wl,--start-group  -Wl,--end-group -nostartfiles -Wl,-e_PowerON_Reset -Wl,--cref -Wl,--icf=none -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_bsp/mcu/all" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_bsp/mcu/rl78_g24" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/general" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_pincfg" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_bsp" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_bsp/mcu/rl78_g24/register_access/llvm" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_config" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/r_bsp/board/generic_rl78_g24" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/Config_IICA0" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/Config_UART1" -I"/home/snob/Documents/e2studio/LuxMeter1.0/src/smc_gen/Config_ADC")
	@echo Building target: $@ && clang --target=rl78 @"LuxMeter1.0.elf.in"> LuxMeter1.cref
: 
	rl78-elf-libgen   --compiler-options=-Os,-ffunction-sections,-fdata-sections,-fdiagnostics-parseable-fixits,-fno-strict-aliasing,-mcpu=s3  -o
LuxMeter1.0.srec: LuxMeter1.0.elf
	llvm-objcopy "LuxMeter1.0.elf" -O srec  "LuxMeter1.0.srec"
LuxMeter1.0.siz: LuxMeter1.0.elf
	llvm-size --format=berkeley "LuxMeter1.0.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(JMP_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(P_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(FSY_UPPER_DEPS)  $(CREF)  $(C_DEPS)  $(SRC_DEPS)  $(OBJCOPY)  $(JMP_UPPER_DEPS)  $(PP_DEPS)  $(SX_DEPS)  $(ASM_UPPER_DEPS)  $(OBJS)  $(SX_UPPER_DEPS)  $(ASM_DEPS)  $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)  $(MAP)  $(S_UPPER_DEPS) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  LuxMeter1.0.elf
	-@echo ' '

secondary-outputs: $(OBJCOPY) $(SECONDARY_SIZE)

.PHONY: all clean dependents 

-include ../makefile.targets
