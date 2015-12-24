################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/rdkafka/rdkafkabase.cpp \
../src/include/rdkafka/rdkafkaconsumer.cpp \
../src/include/rdkafka/rdkafkaproducer.cpp 

OBJS += \
./src/include/rdkafka/rdkafkabase.o \
./src/include/rdkafka/rdkafkaconsumer.o \
./src/include/rdkafka/rdkafkaproducer.o 

CPP_DEPS += \
./src/include/rdkafka/rdkafkabase.d \
./src/include/rdkafka/rdkafkaconsumer.d \
./src/include/rdkafka/rdkafkaproducer.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/rdkafka/%.o: ../src/include/rdkafka/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


