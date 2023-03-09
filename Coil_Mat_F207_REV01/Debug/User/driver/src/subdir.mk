################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/driver/src/eeprom.c \
../User/driver/src/lcd_driver.c 

OBJS += \
./User/driver/src/eeprom.o \
./User/driver/src/lcd_driver.o 

C_DEPS += \
./User/driver/src/eeprom.d \
./User/driver/src/lcd_driver.d 


# Each subdirectory must supply rules for building sources it contributes
User/driver/src/%.o User/driver/src/%.su User/driver/src/%.cyclo: ../User/driver/src/%.c User/driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/Inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/fonts" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/images" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-driver-2f-src

clean-User-2f-driver-2f-src:
	-$(RM) ./User/driver/src/eeprom.cyclo ./User/driver/src/eeprom.d ./User/driver/src/eeprom.o ./User/driver/src/eeprom.su ./User/driver/src/lcd_driver.cyclo ./User/driver/src/lcd_driver.d ./User/driver/src/lcd_driver.o ./User/driver/src/lcd_driver.su

.PHONY: clean-User-2f-driver-2f-src

