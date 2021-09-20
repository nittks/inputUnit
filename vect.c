
#include <avr/interrupt.h>
#include <avr/io.h>
#include "vect_inc.h"
#include "main.h"
#include "drvUart.h"
#include "drvInPalseCnt.h"
#include "drvEep.h"
#include "drvInTimer.h"
#include "timer.h"

#define INT_CNT_MAX	((unsigned char)125)	//8us*125=1ms�����荞��

ISR(TIMER0_COMPA_vect)	//�^�C�}���荞��(�V�X�e���^�C�}
{
	interMainTask();
}
ISR(USART_RX_vect)		//UART��M������
{
	interGetUartRxData();
}

ISR(USART_UDRE_vect)	//UART DataRegisterEmpty���M���W�X�^�󊄍���
{
	interSetUartTxData();
}
ISR( USART_TX_vect ){	//UART���M����������
	interUartTxFin();
}

ISR(TIMER1_OVF_vect)	//�^�C�}�I�[�o�[�t���[(�p���X�Ԋu�v���^�C�}
{
	interDrvInTimerOverflow();
}
ISR(INT0_vect)			//INT0�|�[�g�ω����荞��
{
	interPosEdgeSpeed();
}
ISR(INT1_vect)			//INT1�|�[�g�ω����荞��
{
	interPosEdgeRev();
}
ISR(EE_READY_vect){		//EEPROM�A�N�Z�X�\������
	interEepRedy();
}
