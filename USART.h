#include "stm32f407xx.h"
void UART_Config(void); 
void UART_PutChar(char c);
void UART_PutString(char *s); 
uint16_t UART_GetChar(void); 
void UART_PutNumber(char x);