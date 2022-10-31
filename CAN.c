#include "stm32f407xx.h" 
#include "Can.h"
/*-------------------------------------------------------------------------
CAN Initialization
-------------------------------------------------------------------------*/
void CAN_init()
{
CAN1->MCR &= 0X00; 
CAN1->MSR &= 0X00;
CAN1->MSR |= (0X01<<3); //Wake up is cleared
CAN1->MCR |=0X01; // To enter initialization mode 
INRQ=1; BIT 1
while (!(CAN1->MSR & CAN_MSR_INAK)); //waiting for acknowledgement 
CAN1->MCR |= (0X01<<6); //Automatic Bus Off Management ABOM=1,BIT 6 
CAN1->MCR |= (0X01<<4); //No automatic retransmission
//BIT TIMING PARAMETERS SJW=3, TS1=15, TS2=4, BRP=3,LOOPBACK(bit 30)=1 
CAN1->BTR |= ((0X01<<30)|((0x03) << 24) | ((0x04) << 20) | ((0x0F) << 16) | (0x03));
CAN1->MCR &= 0XFFFFFFFE; //To exit initialization mode 
while(CAN1->MSR & CAN_MSR_INAK);
}
/*-------------------------------------------------------------------------
CAN Filter Initialization
-------------------------------------------------------------------------*/
void CAN_filter_init(void)
{
CAN1->FMR |=(0X01); //Reception is disabled FNIT=1; 
CAN1->FS1R |= 0x00; //Use dual 16-bit scale configuration
CAN1->FM1R = 0; //Use Identifier mask mode
CAN1->FFA1R = 0; //FIFO 0 is activated
CAN1->FA1R |=(0X01<<0); //Required filter will be activated 
CAN1->sFilterRegister[0].FR1 = 0x00000000;
CAN1->FMR &= 0XFFFFFFFE; //Exit Filter Initialization FNIT=0;
}
/*-------------------------------------------------------------------------
CAN Transmit
-------------------------------------------------------------------------*/
uint8_t CAN_transmit(CanTxMsg* TxMsg)
{
uint8_t Tx_mail;
if(CAN1->TSR & CAN_TSR_TME0)
Tx_mail = 0; //Mailbox 0 is empty 
else if (CAN1->TSR&CAN_TSR_TME1)
Tx_mail = 1; //Mailbox 1 is empty 
else if (CAN1->TSR&CAN_TSR_TME2)
Tx_mail = 2; //Mailbox 2 is empty
else
Tx_mail = NoTxMailBox;
if(Tx_mail == 0 || Tx_mail == 1 || Tx_mail == 2)
{
//Setup Transmit Message
//Setup ID
CAN1->sTxMailBox[Tx_mail].TIR &= 0x00;
CAN1->sTxMailBox[Tx_mail].TIR |= ((TxMsg->StdId << 21)|(TxMsg->Rtr));
//Setup DLC
TxMsg -> Dlc &= (uint8_t)0x0000000F;
CAN1->sTxMailBox[Tx_mail].TDTR &= (uint32_t)0xFFFFFFF0; 
CAN1->sTxMailBox[Tx_mail].TDTR |= TxMsg -> Dlc;
CAN1->sTxMailBox[Tx_mail].TDLR = (((uint32_t)TxMsg->Data[3]<<24) |
((uint32_t)TxMsg->Data[2]<<16) | 
((uint32_t)TxMsg->Data[1]<<8) | 
((uint32_t)TxMsg->Data[0]));
CAN1->sTxMailBox[Tx_mail].TDHR = (((uint32_t)TxMsg->Data[7]<<24)|
((uint32_t)TxMsg->Data[6]<<16)| 
((uint32_t)TxMsg->Data[5]<<8)| 
((uint32_t)TxMsg->Data[4]));
//Request Transmission
CAN1->sTxMailBox[Tx_mail].TIR |= CAN_TI0R_TXRQ;
}
return Tx_mail;
}

/*-------------------------------------------------------------------------
CAN Transmit Status
-------------------------------------------------------------------------*/
uint8_t CAN_tx_status(uint8_t Tx_mail)
{
uint32_t state = 0; 
switch(Tx_mail)
{
case 0: if((CAN1->TSR & CAN_TSR_RQCP0) && 
(CAN1->TSR & CAN_TSR_TME0))
{
}
else
if(CAN1->TSR & CAN_TSR_TXOK0)
state = TxSuccess; //Transmission Successful 
else
state = TxFailed; //Transmission Failed
state = TxPending; //Transmission Pending

//Setup Data Field
break;
case 1: if((CAN1->TSR & 
CAN_TSR_RQCP1) && (CAN1->TSR & 
CAN_TSR_TME1))
{
if(CAN1->TSR & CAN_TSR_TXOK1)
state = TxSuccess;
}
else
else
state = TxFailed;
state = TxPending;
break;
case 2: if((CAN1->TSR & CAN_TSR_RQCP2) 
&& (CAN1->TSR & CAN_TSR_TME2))
{
if(CAN1->TSR & CAN_TSR_TXOK2)
state = TxSuccess;
}
else
else
state = TxFailed;
state = TxPending;
break;
default: state = TxFailed; //Transmission Failed
}
return state;
}


/*-------------------------------------------------------------------------
CAN Abort by user
-------------------------------------------------------------------------*/
void Can_Abort_Tx(uint8_t Tx_mail)
{
switch(Tx_mail)
{
case 0: CAN1->TSR |= CAN_TSR_ABRQ0; //Abort Transmission of mailbox 0 
break;
case 1: CAN1->TSR |= CAN_TSR_ABRQ1; //Abort Transmission of mailbox 1 
break;
case 2: CAN1->TSR |= CAN_TSR_ABRQ2; //Abort Transmission of mailbox 2 
break;
default: break;
}
}


/*-------------------------------------------------------------------------
CAN Receive
-------------------------------------------------------------------------*/
void CAN_receive(CanRxMsg* RxMsg)
{
RxMsg->Ide = (uint8_t)0x04 & CAN1->sFIFOMailBox[0].RIR;
RxMsg->StdId = (uint32_t)0x000007FF &
(CAN1->sFIFOMailBox[0].RIR >> 21); //Read Standard ID
//Read type of frame
RxMsg->Rtr = (uint8_t)0x02 & CAN1->sFIFOMailBox[0].RIR;
//Read Data length
RxMsg->Dlc = (uint8_t)0x0F & CAN1->sFIFOMailBox[0].RDTR; 
RxMsg->FMI = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDTR>>8);
//Read Data
RxMsg->Data[0] = (uint8_t)0xFF & CAN1->sFIFOMailBox[0].RDLR; 
RxMsg->Data[1] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR>>8); 
RxMsg->Data[2] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR>>16); 
RxMsg->Data[3] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR>>24); 
RxMsg->Data[4] = (uint8_t)0xFF & CAN1->sFIFOMailBox[0].RDHR; 
RxMsg->Data[5] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR>>8); 
RxMsg->Data[6] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR>>16);
RxMsg->Data[7] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR>>24); 
CAN1->RF0R |= CAN_RF0R_RFOM0; //Release FIFO0
}


/*-------------------------------------------------------------------------
CAN Message Pending in FIFO0
-------------------------------------------------------------------------*/
uint8_t CAN_Msg_Pending(void)
{
uint8_t msg_pending = 0;
msg_pending = (uint8_t)CAN1->RF0R & (uint32_t)0x03; //FMP0[1:0] 
return msg_pending;
}
