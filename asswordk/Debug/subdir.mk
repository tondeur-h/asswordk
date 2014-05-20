################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Console.cpp \
../DBFile.cpp \
../awkBlowfish.cpp \
../main.cpp \
../tools.cpp 

OBJS += \
./Console.o \
./DBFile.o \
./awkBlowfish.o \
./main.o \
./tools.o 

CPP_DEPS += \
./Console.d \
./DBFile.d \
./awkBlowfish.d \
./main.d \
./tools.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


