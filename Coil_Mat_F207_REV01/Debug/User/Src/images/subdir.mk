################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/images/images.c \
../User/Src/images/img_heat_mat.c \
../User/Src/images/img_heating.c \
../User/Src/images/img_test.c 

OBJS += \
./User/Src/images/images.o \
./User/Src/images/img_heat_mat.o \
./User/Src/images/img_heating.o \
./User/Src/images/img_test.o 

C_DEPS += \
./User/Src/images/images.d \
./User/Src/images/img_heat_mat.d \
./User/Src/images/img_heating.d \
./User/Src/images/img_test.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/images/%.o User/Src/images/%.su: ../User/Src/images/%.c User/Src/images/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F207xx -c -I../Core/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/common" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/common/inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/display" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/driver" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/driver/inc" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Src/hangul" -I"D:/Workspace/CubeIDE/Heating_Mat/Heating_Mat_F207/Heating_Mat_F207_REV00/User/Src/images" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Src-2f-images

clean-User-2f-Src-2f-images:
	-$(RM) ./User/Src/images/images.d ./User/Src/images/images.o ./User/Src/images/images.su ./User/Src/images/img_heat_mat.d ./User/Src/images/img_heat_mat.o ./User/Src/images/img_heat_mat.su ./User/Src/images/img_heating.d ./User/Src/images/img_heating.o ./User/Src/images/img_heating.su ./User/Src/images/img_test.d ./User/Src/images/img_test.o ./User/Src/images/img_test.su

.PHONY: clean-User-2f-Src-2f-images

