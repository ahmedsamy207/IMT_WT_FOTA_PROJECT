################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EXTI_program.c \
../src/GPIO_program.c \
../src/MFPEC_Program.c \
../src/MRCC_program.c \
../src/MUSART_Program.c \
../src/NVIC_program.c \
../src/SPARSER_Program.c \
../src/SYSTICK_program.c \
../src/main.c 

OBJS += \
./src/EXTI_program.o \
./src/GPIO_program.o \
./src/MFPEC_Program.o \
./src/MRCC_program.o \
./src/MUSART_Program.o \
./src/NVIC_program.o \
./src/SPARSER_Program.o \
./src/SYSTICK_program.o \
./src/main.o 

C_DEPS += \
./src/EXTI_program.d \
./src/GPIO_program.d \
./src/MFPEC_Program.d \
./src/MRCC_program.d \
./src/MUSART_Program.d \
./src/NVIC_program.d \
./src/SPARSER_Program.d \
./src/SYSTICK_program.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


