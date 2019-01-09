#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>

const uint32_t Serial_Buffer_Size = 1000;
const uint32_t Json_Message_Buffer_Size = 4000;
extern const uint32_t Baudrate;

extern const int32_t Number_Of_Pwm;
extern const int32_t Number_Of_Device;

extern const uint16_t Max_Pwm_Value;
extern const uint16_t Min_Pwm_Value;

extern const uint8_t Enable_Pin;
extern const uint8_t Latch_Pin;
extern const uint8_t Data_Pin;
extern const uint8_t Clock_Pin;


#endif
