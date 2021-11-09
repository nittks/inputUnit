
#include <avr/interrupt.h>
#include <avr/io.h>
#include "vect_inc.h"
#include "main.h"
#include "drvUart.h"
#include "drvInPalseCnt.h"
#include "drvEep.h"
#include "drvInTimer.h"
#include "timer.h"

#define INT_CNT_MAX	((unsigned char)125)	//8us*125=1ms毎割り込み

ISR(TCA0_OVF_vect)	//タイマ割り込み
{
	interTaskTime();
}
ISR(TCA0_CMP0_vect)	//タイマ割り込み
{
	interTaskTime();
}
ISR(USART0_RXC_vect)		//UART受信割込み
{
	interGetUartRxData();
}
ISR(USART1_DRE_vect)	//UART DataRegisterEmpty送信レジスタ空割込み
{
	interSetUartTxData();
}
ISR( USART1_TXC_vect ){	//UART送信完了割込み
	interUartTxFin();
}

ISR(TCB0_INT_vect)	//タイマオーバーフロー(パルス間隔計測タイマ
{
	interDrvInTimerOverflow();
}
ISR(PORTF_PORT_vect)			//ポートF割り込み
{
	interPosEdgeSpeed();
}
ISR(NVMCTRL_EE_vect){		//EEPROMアクセス可能割込み
	interEepRedy();
}
