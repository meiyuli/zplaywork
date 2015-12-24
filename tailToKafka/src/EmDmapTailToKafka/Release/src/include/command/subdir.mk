################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/command/commandmanager.cpp 

OBJS += \
./src/include/command/commandmanager.o 

CPP_DEPS += \
./src/include/command/commandmanager.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/command/%.o: ../src/include/command/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


