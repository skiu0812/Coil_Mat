################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/display/lcd/fonts/fonts.c \
../User/display/lcd/fonts/fonts_0506.c \
../User/display/lcd/fonts/fonts_gulim.c \
../User/display/lcd/fonts/fonts_hyTaebaek.c \
../User/display/lcd/fonts/fonts_yugothic.c \
../User/display/lcd/fonts/han.c 

OBJS += \
./User/display/lcd/fonts/fonts.o \
./User/display/lcd/fonts/fonts_0506.o \
./User/display/lcd/fonts/fonts_gulim.o \
./User/display/lcd/fonts/fonts_hyTaebaek.o \
./User/display/lcd/fonts/fonts_yugothic.o \
./User/display/lcd/fonts/han.o 

C_DEPS += \
./User/display/lcd/fonts/fonts.d \
./User/display/lcd/fonts/fonts_0506.d \
./User/display/lcd/fonts/fonts_gulim.d \
./User/display/lcd/fonts/fonts_hyTaebaek.d \
./User/display/lcd/fonts/fonts_yugothic.d \
./User/display/lcd/fonts/han.d 


# Each subdirectory must supply rules for building sources it contributes
User/display/lcd/fonts/%.o User/display/lcd/fonts/%.su User/display/lcd/fonts/%.cyclo: ../User/display/lcd/fonts/%.c User/display/lcd/fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/Inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/fonts" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/images" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-display-2f-lcd-2f-fonts

clean-User-2f-display-2f-lcd-2f-fonts:
	-$(RM) ./User/display/lcd/fonts/fonts.cyclo ./User/display/lcd/fonts/fonts.d ./User/display/lcd/fonts/fonts.o ./User/display/lcd/fonts/fonts.su ./User/display/lcd/fonts/fonts_0506.cyclo ./User/display/lcd/fonts/fonts_0506.d ./User/display/lcd/fonts/fonts_0506.o ./User/display/lcd/fonts/fonts_0506.su ./User/display/lcd/fonts/fonts_gulim.cyclo ./User/display/lcd/fonts/fonts_gulim.d ./User/display/lcd/fonts/fonts_gulim.o ./User/display/lcd/fonts/fonts_gulim.su ./User/display/lcd/fonts/fonts_hyTaebaek.cyclo ./User/display/lcd/fonts/fonts_hyTaebaek.d ./User/display/lcd/fonts/fonts_hyTaebaek.o ./User/display/lcd/fonts/fonts_hyTaebaek.su ./User/display/lcd/fonts/fonts_yugothic.cyclo ./User/display/lcd/fonts/fonts_yugothic.d ./User/display/lcd/fonts/fonts_yugothic.o ./User/display/lcd/fonts/fonts_yugothic.su ./User/display/lcd/fonts/han.cyclo ./User/display/lcd/fonts/han.d ./User/display/lcd/fonts/han.o ./User/display/lcd/fonts/han.su

.PHONY: clean-User-2f-display-2f-lcd-2f-fonts

