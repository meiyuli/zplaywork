################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/WaitQuitSignal/WaitQuitSignal.cpp 

OBJS += \
./src/include/WaitQuitSignal/WaitQuitSignal.o 

CPP_DEPS += \
./src/include/WaitQuitSignal/WaitQuitSignal.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/WaitQuitSignal/%.o: ../src/include/WaitQuitSignal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


