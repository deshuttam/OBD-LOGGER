#include "stm32f407xx.h" 
#include "Can.h" 
#include "uart.h"
#include "obd.h"
void delay(int d)
{
for(int i = 0; i<d; i++);
}
int main()
{
uint32_t counter = 0; 
CanTxMsg TxMsg; 
CanRxMsg RxMsg; 
uint8_t Tx_mailbox; 
UART_Config();
//Clock Configuration
RCC->AHB1ENR |= (0x01<<1); //GPIOB Clock Enable 
RCC->APB1ENR |= (0x01<<25); //CAN1 is activated with 42MHz Clock
//PB8 and PB9 for alternate function
GPIOB->MODER &= ~((3<<(8*2))|(3<<(9*2)));
GPIOB->MODER |= ((2<<(8*2))|(2<<(9*2)));
GPIOB->OTYPER &= ~((1<<8)|(1<<9)); //for output push pull CAN Tx-Rx
//Low Speed[Bits 31..0] RO (@ 0x400065B8) mailbox data high register 
GPIOB->OSPEEDR &= ~((3<<(8*2))|(3<<(9*2)));
GPIOB->PUPDR &= ~((3<<(8*2))|(3<<(9*2))); //No pull-up and pull-down 
GPIOB->AFR[1] &= ~((15<<(0*4))|(15<<(1*4))); //AF9 for CAN 
GPIOB->AFR[1] |= ((9<<(0*4))|(9<<(1*4)));
CAN_init(); 
CAN_filter_init(); 
while(1){
delay(1000);
uint8_t data = UART_GetChar();
//Transit Message 
TxMsg.StdId = 0x7EF; 
TxMsg.Ide = 0; 
TxMsg.Rtr = 0x00; 
TxMsg.Dlc = 8; 
TxMsg.Data[0] = 0x01; 
TxMsg.Data[1] = data; 
TxMsg.Data[2] = 0x00; 
TxMsg.Data[3] = 0x00; 
TxMsg.Data[4] = 0x00; 
TxMsg.Data[5] = 0x00; 
TxMsg.Data[6] = 0x00; 
TxMsg.Data[7] = 0x00;
Tx_mailbox = CAN_transmit(&TxMsg);
//Waiting for Transmission 
while((CAN_tx_status(Tx_mailbox)!= (TxSuccess))
&& (counter!=0xFFFF))
{
counter++;
}
counter = 0;
//Polling for reception
while((CAN_Msg_Pending() < 1) && (counter!=0xFFFF))
{
counter++;
}
RxMsg.StdId = 0x00; 
RxMsg.Ide = 0x00; 
RxMsg.Dlc = 0; 
RxMsg.Data[0] = 0x00; 
RxMsg.Data[1] = 0x00; 
RxMsg.Data[2] = 0x00; 
RxMsg.Data[3] = 0x00; 
RxMsg.Data[4] = 0x00; 
RxMsg.Data[5] = 0x00; 
RxMsg.Data[6] = 0x00; 
RxMsg.Data[7] = 0x00;
CAN_receive(&RxMsg); 
uint8_t pid=RxMsg.Data[1]; 
uint8_t a = RxMsg.Data[2]; 
uint8_t b = RxMsg.Data[3];
if(pid== 0x0D)
{
Vehicle_Speed(a);
}


else if(pid == 0x0C)
{
Engine_RPM(a, b);
}
else if(pid == 0x05)
{
Engine_Coolant_Temp(a);
}
else if(pid == 0x11)
{
Throttle_Position(a);
}
else if(pid == 0x0A)
{
Fuel_Pressure(a);
}
else if(pid == 0x10)
{
MAF_Rate(a, b);
}
}
}
