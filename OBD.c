#include "stm32f407xx.h" 
#include "uart.h"
#include "obd.h"
void Vehicle_Speed(uint8_t a)
{
UART_PutNumber(a);
}
void Engine_RPM(uint8_t a, uint8_t b)
{
uint16_t res = ((256*a)+b)/4; 
uint16_t rev=0; 
while(res!=0)
{
uint8_t r1 = res%10; 
res = res/10;
rev = rev * 10 + r1;
}
while(rev!=0)
{
uint8_t r1 = rev%10; 
rev=rev/10; 
UART_PutNumber(r1);
}
}
void Engine_Coolant_Temp(uint8_t a)
{
UART_PutNumber(a-40);
}
void Throttle_Position(uint8_t a)
{
uint8_t res = (100*a)/255; 
UART_PutNumber(res);
}
void Fuel_Pressure(uint8_t a)
{
UART_PutNumber(3*a);
}
void MAF_Rate(uint8_t a, uint8_t b)
{
uint16_t res = ((256*a)+b)/100; 
UART_PutNumber(res);
}
