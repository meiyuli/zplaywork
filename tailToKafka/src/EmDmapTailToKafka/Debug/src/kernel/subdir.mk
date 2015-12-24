################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/kernel/EmDmapManager.cpp \
../src/kernel/EmDmapReadFile.cpp \
../src/kernel/EmDmapWriteKafka.cpp \
../src/kernel/FileManager.cpp 

OBJS += \
./src/kernel/EmDmapManager.o \
./src/kernel/EmDmapReadFile.o \
./src/kernel/EmDmapWriteKafka.o \
./src/kernel/FileManager.o 

CPP_DEPS += \
./src/kernel/EmDmapManager.d \
./src/kernel/EmDmapReadFile.d \
./src/kernel/EmDmapWriteKafka.d \
./src/kernel/FileManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/kernel/%.o: ../src/kernel/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


