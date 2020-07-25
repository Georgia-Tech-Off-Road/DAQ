################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Test\ Struct.cpp \
../TestStruct2.cpp 

OBJS += \
./Test\ Struct.o \
./TestStruct2.o 

CPP_DEPS += \
./Test\ Struct.d \
./TestStruct2.d 


# Each subdirectory must supply rules for building sources it contributes
Test\ Struct.o: ../Test\ Struct.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Test Struct.d" -MT"Test\ Struct.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


