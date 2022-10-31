#include "stm32f407xx.h" 
#include "uart.h" 
#include<stdio.h>
void UART_Config()
{
RCC->AHB1ENR |= (0x01<<2); // Enable clock for GPIOC 
RCC->APB1ENR |= (0x01<<19); // Enable clock for USART
GPIOC->MODER &= ~((3<<(10*2))|(3<<(11*2))); // Initialization of GPIOC
//PC10 and PC11 for alternate function
GPIOC->MODER |= ((2<<(10*2))|(2<<(11*2)));
//for output push pull USART Tx-Rx 
GPIOC->OTYPER &= ~((1<<10)|(1<<11));
GPIOC->OSPEEDR &= ~((3<<(10*2))|(3<<(11*2))); //Low Speed 
GPIOC->PUPDR &= ~((3<<(10*2))|(3<<(11*2))); //Pull-up 
GPIOC->PUPDR |= ((1<<(10*2))|(1<<(11*2)));
GPIOC->AFR[1] &= ~((8<<(2*4))|(8<<(3*4))); //AF8 for UART 
GPIOC->AFR[1] |= ((8<<(2*4))|(8<<(3*4)));
UART4->CR1 |= (0x00<<12); //M bit 8 bits
UART4->CR2 &= (~(0X03<<12)); //1 stop bit 
UART4->BRR|= (SystemCoreClock/9600); //Baud rate = 9600
//Transmit enable(TE bit),UE bit is set to 1 to enable UART4 and Receive Enable(RE bit) 
UART4->CR1 |= ((0x01<<13) | (0x01<<3) | (1<<2));
}
void UART_PutNumber(char x)
{
char buf[5]; 
unsigned char i; 
sprintf(buf, "%d", x); 
i=0;
while(buf[i]!='\0')
{
// Wait until transmit data register is empty 
while(!(UART4->SR & USART_SR_TXE));
// Send a char using USART1 
UART4->DR = buf[i++];
}
}

void UART_PutChar(char c)
{
// Wait until transmit data register is empty 
while(!(UART4->SR & USART_SR_TXE));
// Send a char using USART1
UART4->DR = (c & (uint8_t)0x01FF);
}
void UART_PutString(char *s)
{
// Send a string 
while (*s)
{
UART_PutChar(*s++);
}
}
uint16_t UART_GetChar()
{
// Wait until data is received
while(!(UART4->SR & USART_SR_RXNE)){}
// Read received char
return (uint8_t)(UART4->DR & (uint8_t)0x01FF);
}
