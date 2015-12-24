################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/logstat/logstat.cpp 

OBJS += \
./src/include/logstat/logstat.o 

CPP_DEPS += \
./src/include/logstat/logstat.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/logstat/%.o: ../src/include/logstat/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


