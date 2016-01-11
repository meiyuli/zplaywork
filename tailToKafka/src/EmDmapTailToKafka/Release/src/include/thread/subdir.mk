################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/thread/EmThread.cpp 

OBJS += \
./src/include/thread/EmThread.o 

CPP_DEPS += \
./src/include/thread/EmThread.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/thread/%.o: ../src/include/thread/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


