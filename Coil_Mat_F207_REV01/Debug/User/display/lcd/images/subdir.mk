################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/display/lcd/images/image_mat.c \
../User/display/lcd/images/image_mat_heating.c \
../User/display/lcd/images/image_test.c \
../User/display/lcd/images/images.c 

OBJS += \
./User/display/lcd/images/image_mat.o \
./User/display/lcd/images/image_mat_heating.o \
./User/display/lcd/images/image_test.o \
./User/display/lcd/images/images.o 

C_DEPS += \
./User/display/lcd/images/image_mat.d \
./User/display/lcd/images/image_mat_heating.d \
./User/display/lcd/images/image_test.d \
./User/display/lcd/images/images.d 


# Each subdirectory must supply rules for building sources it contributes
User/display/lcd/images/%.o User/display/lcd/images/%.su User/display/lcd/images/%.cyclo: ../User/display/lcd/images/%.c User/display/lcd/images/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/Inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/common/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/fonts" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/images" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/display/lcd/inc" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver" -I"D:/Workspace/CubeIDE/Coil_Mat/Coil_Mat_F207/Coil_Mat_F207_REV01/User/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-display-2f-lcd-2f-images

clean-User-2f-display-2f-lcd-2f-images:
	-$(RM) ./User/display/lcd/images/image_mat.cyclo ./User/display/lcd/images/image_mat.d ./User/display/lcd/images/image_mat.o ./User/display/lcd/images/image_mat.su ./User/display/lcd/images/image_mat_heating.cyclo ./User/display/lcd/images/image_mat_heating.d ./User/display/lcd/images/image_mat_heating.o ./User/display/lcd/images/image_mat_heating.su ./User/display/lcd/images/image_test.cyclo ./User/display/lcd/images/image_test.d ./User/display/lcd/images/image_test.o ./User/display/lcd/images/image_test.su ./User/display/lcd/images/images.cyclo ./User/display/lcd/images/images.d ./User/display/lcd/images/images.o ./User/display/lcd/images/images.su

.PHONY: clean-User-2f-display-2f-lcd-2f-images

