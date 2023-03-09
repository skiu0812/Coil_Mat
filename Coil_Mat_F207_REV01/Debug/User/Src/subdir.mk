################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/adc_api.c \
../User/Src/adc_mid.c \
../User/Src/button_api.c \
../User/Src/button_mid.c \
../User/Src/cli_mid.c \
../User/Src/coil_mat_api.c \
../User/Src/coil_mid.c \
../User/Src/coil_solenoid_api.c \
../User/Src/eeprom_mid.c \
../User/Src/flash_mid.c \
../User/Src/gpio_mid.c \
../User/Src/i2c_mid.c \
../User/Src/led_api.c \
../User/Src/led_mid.c \
../User/Src/main_api.c \
../User/Src/main_mid.c \
../User/Src/spi_mid.c \
../User/Src/test_mid.c \
../User/Src/tim_mid.c \
../User/Src/uart_api.c \
../User/Src/uart_mid.c \
../User/Src/usb_api.c \
../User/Src/usb_mid.c \
../User/Src/user.c 

OBJS += \
./User/Src/adc_api.o \
./User/Src/adc_mid.o \
./User/Src/button_api.o \
./User/Src/button_mid.o \
./User/Src/cli_mid.o \
./User/Src/coil_mat_api.o \
./User/Src/coil_mid.o \
./User/Src/coil_solenoid_api.o \
./User/Src/eeprom_mid.o \
./User/Src/flash_mid.o \
./User/Src/gpio_mid.o \
./User/Src/i2c_mid.o \
./User/Src/led_api.o \
./User/Src/led_mid.o \
./User/Src/main_api.o \
./User/Src/main_mid.o \
./User/Src/spi_mid.o \
./User/Src/test_mid.o \
./User/Src/tim_mid.o \
./User/Src/uart_api.o \
./User/Src/uart_mid.o \
./User/Src/usb_api.o \
./User/Src/usb_mid.o \
./User/Src/user.o 

C_DEPS += \
./User/Src/adc_api.d \
./User/Src/adc_mid.d \
./User/Src/button_api.d \
./User/Src/button_mid.d \
./User/Src/cli_mid.d \
./User/Src/coil_mat_api.d \
./User/Src/coil_mid.d \
./User/Src/coil_solenoid_api.d \
./User/Src/eeprom_mid.d \
./User/Src/flash_mid.d \
./User/Src/gpio_mid.d \
./User/Src/i2c_mid.d \
./User/Src/led_api.d \
./User/Src/led_mid.d \
./User/Src/main_api.d \
./User/Src/main_mid.d \
./User/Src/spi_mid.d \
./User/Src/test_mid.d \
./User/Src/tim_mid.d \
./User/Src/uart_api.d \
./User/Src/uart_mid.d \
./User/Src/usb_api.d \
./User/Src/usb_mid.d \
./User/Src/user.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/Inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/fonts" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/images" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/adc_api.cyclo ./User/Src/adc_api.d ./User/Src/adc_api.o ./User/Src/adc_api.su ./User/Src/adc_mid.cyclo ./User/Src/adc_mid.d ./User/Src/adc_mid.o ./User/Src/adc_mid.su ./User/Src/button_api.cyclo ./User/Src/button_api.d ./User/Src/button_api.o ./User/Src/button_api.su ./User/Src/button_mid.cyclo ./User/Src/button_mid.d ./User/Src/button_mid.o ./User/Src/button_mid.su ./User/Src/cli_mid.cyclo ./User/Src/cli_mid.d ./User/Src/cli_mid.o ./User/Src/cli_mid.su ./User/Src/coil_mat_api.cyclo ./User/Src/coil_mat_api.d ./User/Src/coil_mat_api.o ./User/Src/coil_mat_api.su ./User/Src/coil_mid.cyclo ./User/Src/coil_mid.d ./User/Src/coil_mid.o ./User/Src/coil_mid.su ./User/Src/coil_solenoid_api.cyclo ./User/Src/coil_solenoid_api.d ./User/Src/coil_solenoid_api.o ./User/Src/coil_solenoid_api.su ./User/Src/eeprom_mid.cyclo ./User/Src/eeprom_mid.d ./User/Src/eeprom_mid.o ./User/Src/eeprom_mid.su ./User/Src/flash_mid.cyclo ./User/Src/flash_mid.d ./User/Src/flash_mid.o ./User/Src/flash_mid.su ./User/Src/gpio_mid.cyclo ./User/Src/gpio_mid.d ./User/Src/gpio_mid.o ./User/Src/gpio_mid.su ./User/Src/i2c_mid.cyclo ./User/Src/i2c_mid.d ./User/Src/i2c_mid.o ./User/Src/i2c_mid.su ./User/Src/led_api.cyclo ./User/Src/led_api.d ./User/Src/led_api.o ./User/Src/led_api.su ./User/Src/led_mid.cyclo ./User/Src/led_mid.d ./User/Src/led_mid.o ./User/Src/led_mid.su ./User/Src/main_api.cyclo ./User/Src/main_api.d ./User/Src/main_api.o ./User/Src/main_api.su ./User/Src/main_mid.cyclo ./User/Src/main_mid.d ./User/Src/main_mid.o ./User/Src/main_mid.su ./User/Src/spi_mid.cyclo ./User/Src/spi_mid.d ./User/Src/spi_mid.o ./User/Src/spi_mid.su ./User/Src/test_mid.cyclo ./User/Src/test_mid.d ./User/Src/test_mid.o ./User/Src/test_mid.su ./User/Src/tim_mid.cyclo ./User/Src/tim_mid.d ./User/Src/tim_mid.o ./User/Src/tim_mid.su ./User/Src/uart_api.cyclo ./User/Src/uart_api.d ./User/Src/uart_api.o ./User/Src/uart_api.su ./User/Src/uart_mid.cyclo ./User/Src/uart_mid.d ./User/Src/uart_mid.o ./User/Src/uart_mid.su ./User/Src/usb_api.cyclo ./User/Src/usb_api.d ./User/Src/usb_api.o ./User/Src/usb_api.su ./User/Src/usb_mid.cyclo ./User/Src/usb_mid.d ./User/Src/usb_mid.o ./User/Src/usb_mid.su ./User/Src/user.cyclo ./User/Src/user.d ./User/Src/user.o ./User/Src/user.su

.PHONY: clean-User-2f-Src

