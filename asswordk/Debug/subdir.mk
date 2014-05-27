################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AES.cpp \
../Console.cpp \
../DBFile.cpp \
../Timer.cpp \
../awkBlowfish.cpp \
../main.cpp \
../tools.cpp 

OBJS += \
./AES.o \
./Console.o \
./DBFile.o \
./Timer.o \
./awkBlowfish.o \
./main.o \
./tools.o 

CPP_DEPS += \
./AES.d \
./Console.d \
./DBFile.d \
./Timer.d \
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


