################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/buzzer.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/control_main.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/dc_motor.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/external_eeprom.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/gpio.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/timer.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/twi.c \
C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/uart.c 

OBJS += \
./CONTROL_ECU/buzzer.o \
./CONTROL_ECU/control_main.o \
./CONTROL_ECU/dc_motor.o \
./CONTROL_ECU/external_eeprom.o \
./CONTROL_ECU/gpio.o \
./CONTROL_ECU/timer.o \
./CONTROL_ECU/twi.o \
./CONTROL_ECU/uart.o 

C_DEPS += \
./CONTROL_ECU/buzzer.d \
./CONTROL_ECU/control_main.d \
./CONTROL_ECU/dc_motor.d \
./CONTROL_ECU/external_eeprom.d \
./CONTROL_ECU/gpio.d \
./CONTROL_ECU/timer.d \
./CONTROL_ECU/twi.d \
./CONTROL_ECU/uart.d 


# Each subdirectory must supply rules for building sources it contributes
CONTROL_ECU/buzzer.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/buzzer.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/control_main.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/control_main.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/dc_motor.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/dc_motor.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/external_eeprom.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/external_eeprom.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/gpio.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/gpio.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/timer.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/timer.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/twi.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/twi.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CONTROL_ECU/uart.o: C:/Users/h4z3m/Desktop/Files/ES\ Diploma\ 56/Projects/Final_Project/CONTROL_ECU/uart.c CONTROL_ECU/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


