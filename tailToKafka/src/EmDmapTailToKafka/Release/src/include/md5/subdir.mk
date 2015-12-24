################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/md5/md5.cpp 

OBJS += \
./src/include/md5/md5.o 

CPP_DEPS += \
./src/include/md5/md5.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/md5/%.o: ../src/include/md5/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


