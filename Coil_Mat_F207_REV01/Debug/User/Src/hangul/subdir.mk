################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/hangul/han.c 

OBJS += \
./User/Src/hangul/han.o 

C_DEPS += \
./User/Src/hangul/han.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/hangul/%.o User/Src/hangul/%.su: ../User/Src/hangul/%.c User/Src/hangul/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/common" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/common/inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/display" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/driver" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/driver/inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Src/hangul" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Src/images" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Src-2f-hangul

clean-User-2f-Src-2f-hangul:
	-$(RM) ./User/Src/hangul/han.d ./User/Src/hangul/han.o ./User/Src/hangul/han.su

.PHONY: clean-User-2f-Src-2f-hangul

