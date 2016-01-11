################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/threadpool/threadpool.cpp 

OBJS += \
./src/include/threadpool/threadpool.o 

CPP_DEPS += \
./src/include/threadpool/threadpool.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/threadpool/%.o: ../src/include/threadpool/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


