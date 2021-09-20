
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //���荞�݂��g�p���邽��
#include <string.h>
#include <stdbool.h>

#include "drvUart_inc.h"
#include "drvUart.h"
#include "hardware.h"
#include "timer.h"

static UART_STATE		uartState;
//----------------------------------------
// ���M
//----------------------------------------
static DRV_UART_TX		drvUartTx;
static unsigned char	txDataCnt;
static unsigned char	txCnt;
static unsigned char	txReqFlag;

//----------------------------------------
// ��M
//----------------------------------------
static DRV_UART_RX		drvUartRx;
static unsigned char	rxDataBuf[DRV_UART_RX_BUF_SIZE];			//��M���f�[�^�����Ă����o�b�t�@
static unsigned char	rxDataBufCnt;		//��M���f�[�^�o�b�t�@�p�J�E���^
static unsigned char	rxDataCnt;			//URAT�f�[�^���J�E���^
static unsigned char	rxDataLen;			//UART�t���[�����擾�����t���[�������O�X
static unsigned char	rxFlag;				//��M�L��

//********************************************************************************//
// ������
//********************************************************************************//
void initDrvUart( void )
{
	unsigned char	i;

	//���M
	for( i=0 ; i<DRV_UART_RX_BUF_SIZE; i++ ){
		drvUartTx.txData[i]	= 0;
	}
	drvUartTx.txDataNum =	0;
	txDataCnt	= 0;
	txCnt		= 0;
	txReqFlag	= false;

	//��M
	for( i=0 ; i<DRV_UART_RX_BUF_SIZE ; i++ ){
		drvUartRx.rxData[i]	= 0;
	}
	drvUartRx.rxDataNum =	0;
	drvUartRx.rxFlag = false;
		
	rxDataCnt		= 0;
	rxDataLen		= 0;

	//����M����
	EN_UART_TX;
	EN_UART_RX;
//	UCSR0B	= (1<<TXEN0);

	//��M���������݋���
	uartState		= UART_STATE_STANDBY;
	EN_INTER_UART_RX_COMP;
	RS485_RX;
}

//********************************************************************************//
// ���M�Z�b�g
//********************************************************************************//
void setDrvUartTx( DRV_UART_TX *inP )
{
	drvUartTx	= *inP;

	if( uartState == UART_STATE_STANDBY ){
		uartState = UART_STATE_TRANS;
		RS485_TX;						//RS485�g�����V�[�o�����ؑ�
		EN_INTER_UART_TX_REG_EMPTY;		//���M�o�b�t�@�󊄍��݋��� 
		DI_INTER_UART_RX_COMP;			//��M���������݋֎~
	}
}

//********************************************************************************//
// ���M���W�X�^�󊄍���
//********************************************************************************//
void interSetUartTxData(void)
{
	cli();	//���荞�݋֎~

	while( UART_REG_UDRE == UDRE_EMPTY ){	//���M���W�X�^��̊ԉ�
		UDR0 = drvUartTx.txData[txDataCnt];
		txDataCnt++;

		if( txDataCnt >= drvUartTx.txDataNum ){	//�S�f�[�^���M�ς�

			txDataCnt = 0;
			DI_INTER_UART_TX_REG_EMPTY;		//���M���W�X�^�󊄍��݋֎~
			EN_INTER_UART_TX_FIN;			//���M���������݋���
		}
	}
	sei();	//�����݋���
}

//********************************************************************************//
// ���M����������
//********************************************************************************//
void interUartTxFin(void)
{
	cli();	//���荞�݋֎~

	uartState = UART_STATE_STANDBY;

	RS485_RX;						//RS485�g�����V�[�o�͎�M��Ԃőҋ@
	EN_INTER_UART_RX_COMP;			//��M���������݋���

	sei();	//�����݋���
}


//********************************************************************************//
// ��M�f�[�^�擾
//********************************************************************************//
DRV_UART_RX *getDrvUartRx( void )
{
	//��M�L���t���O�����J�ϐ��֓���A�N���A����
	drvUartRx.rxFlag = rxFlag;
	rxFlag = false;
	return( &drvUartRx );
}


//********************************************************************************//
// UART��M�f�[�^���荞�ݏ���
//********************************************************************************//
void interGetUartRxData(void)
{
	unsigned char	rxBuf;
	unsigned char	timerCnt;

	cli();	//���荞�݋֎~
	while( UART_REG_RXC == RXC_IN_DATA){

		//���W�X�^���f�[�^�擾
		rxBuf = UDR0;
		
		//�^�C�}�I�[�o�[�t���[ or �t���[���ԃ^�C���A�E�g
		//�t���[���̍ŏ�(ID)�����M���Ȃ���
		timerCnt	= getTimerCnt  ( TIMER_DRV_IN_UART_TIMEOUT );
		if( (timerCnt == TIMER_OVER_FLOW ) || (timerCnt > UART_FRAME_TIMEOUT) ){
			uartState = UART_STATE_STANDBY;	//�ҋ@���փ��Z�b�g
			rxDataCnt=0;
			rxDataBufCnt=0;
		}else{
			//��M�����̂ŁA�^�C���A�E�g�^�C�}�N���A
			clearTimer( TIMER_DRV_IN_UART_TIMEOUT );
		}
		//�f�[�^�`�F�b�N
		if( uartState == UART_STATE_STANDBY){
			//�t���[��ID����
			if(( rxDataCnt == UART_DATAPOS_ID ) &&		//ID�ʒu
			( rxBuf == UART_ID_SETTING )				//ID��v
			){
				uartState = UART_STATE_RECEIVE;	//��M��Ԃֈڍs
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;
			}
		}else if( uartState == UART_STATE_RECEIVE){
			//�f�[�^���擾
			if( rxDataCnt == UART_DATAPOS_LENGTH ){
				rxDataLen = rxBuf;		//�t���[�����L�^
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;

			//�ʏ��M
			}else{
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;
					
				//��M����
				if( rxDataCnt >= rxDataLen ){
					uartState = UART_STATE_STANDBY;
					//Lnk�擾�p�z��փR�s�[
					memcpy( &drvUartRx.rxData[0] , &rxDataBuf[0] , rxDataCnt);
					rxFlag = true;		//��M�L��
					rxDataCnt = 0;
				}
			}
		}else{
			//��蓾�Ȃ�
		}
	}
	sei();	//�����݋���

}


//********************************************************************************//
// UART
// �{�[���[�g=9600bps(9.6b/ms
// ��M�f�[�^=5byte
// �X�^�[�gbit=1,�X�g�b�vbit=2bit,�f�[�^=8bit,�p���e�B=�Ȃ���1byte11bit�f�[�^
// 5x11=55bit���M����
// 55bit/9.6bpms = 5.72ms ��1�t���[���ӂ�A�Œ�6ms�ȏ�͕K�v
//********************************************************************************//
