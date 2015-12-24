################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/include/tinyxml/tinystr.cpp \
../src/include/tinyxml/tinyxml.cpp \
../src/include/tinyxml/tinyxmlerror.cpp \
../src/include/tinyxml/tinyxmlparser.cpp \
../src/include/tinyxml/xml.cpp 

OBJS += \
./src/include/tinyxml/tinystr.o \
./src/include/tinyxml/tinyxml.o \
./src/include/tinyxml/tinyxmlerror.o \
./src/include/tinyxml/tinyxmlparser.o \
./src/include/tinyxml/xml.o 

CPP_DEPS += \
./src/include/tinyxml/tinystr.d \
./src/include/tinyxml/tinyxml.d \
./src/include/tinyxml/tinyxmlerror.d \
./src/include/tinyxml/tinyxmlparser.d \
./src/include/tinyxml/xml.d 


# Each subdirectory must supply rules for building sources it contributes
src/include/tinyxml/%.o: ../src/include/tinyxml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


