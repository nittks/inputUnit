/*
 * periodTask.c
 *
 * Created: 2016/05/06 8:25:08
 *  Author: sin
 */ 

/*
����^�X�N
MPU		:ATtiny2313
clock	:����1MHz
*/

#define F_CPU 8000000UL
#define CNTMAX		((unsigned char)7)	//8bit
#define CNT100MSMAX	((unsigned char)200)	//5ms���荞��20���100ms

//UART��`
#define FOSC	8000000				/* MCU�ۯ����g�� */
#define BAUD	9600				/* �ړIUSART�ްڰđ��x */
//#define BAUD	2400				/* �ړIUSART�ްڰđ��x */
#define MYUBRR	(FOSC/16/BAUD-1)	 /* �ړIUBRR�l */
//#define  MYUBRR		26		// (1MHz / (16+2400)) -1
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

//#include "timer.h"
#include "drvInMain.h"
#include "drvOutMain.h"
#include "lnkInMain.h"
#include "lnkOutMain.h"
#include "aplMain.h"
#include "timer.h"
#include "main_inc.h"
#include "main.h"

#define INT_CNT_MAX	((unsigned char)125)	//8us*125=1ms�����荞��

static void mainTask( void );
static void initReg( void );
static void USART_Init( unsigned short baud );


int main(void)
{
	initMain();

    while(1)
    {
		//����������X���[�v�B�^�C�}���荞�݂ŋN������ēx���[�v�J�n
		mainTask();
		set_sleep_mode(SLEEP_MODE_IDLE);
    }
}

void initMain( void )
{
	initReg();

	initDrvIn();
	initLnkIn();
	initApl();
	initLnkOut();

	initDrvOut();
	
//	initLinkInSpeed();
//	initAplData();
//	initAplDispData();
//	initLnkOutLed();
//	initDrvOutLed();
}
static void mainTask( void )
{
	unsigned char	i;
	
	//�����^�X�N���s���ԃ`�F�b�N
	for( i=0; i<TASK_MAX ; i++){
		if( taskParameter[i].cycleTime != 0 ){	//0�͖��g�p

			//�������Ԍo�߂��Ă����珈��
			if( taskParameter[i].currentTime <= 0 ){
				//���Z�b�g
				taskParameter[i].currentTime = taskParameter[i].cycleTime; 
				//�^�X�N���s
				taskParameter[i].func();
			}
		}
	}
}


//���C���^�X�N(�^�C�}���荞�݋쓮)
void interMainTask( void )
{
	unsigned char	i;
	
	//�����^�X�N���s���ԃ`�F�b�N
	for( i=0; i<TASK_MAX ; i++){
		if( taskParameter[i].cycleTime != 0 ){	//0�͖��g�p

			//10ms��1�J�E���g�_�E���B
			if( taskParameter[i].currentTime > 0 ){
				taskParameter[i].currentTime--;
			}
		}
	}
}



static void initReg(void)
{
	//I/O�ݒ�
	DDRB	= 0x33;
	DDRC	= 0xFF;
	DDRD	= 0x83;

	//�����o�͒l�ݒ�
	PORTB	= 0x0C;
	PORTC	= 0x00;
//	PORTD	= 0xFF;		//���̓v���A�b�v
	PORTD	= 0xF3;		//���̓v���A�b�v


	//�^�C�}0�ACTC�A���荞�ݗp�A��rA��v�Ŋ��荞��
	TCCR0A	= SET_TCCR0A;
	TCCR0B	= SET_TCCR0B;
	OCR0A	= SET_OCR0A;
	TIMSK0	= SET_TIMSK0;
	OSCCAL	= SET_OSCCAL;	//�N���b�N�Z��
	
	USART_Init( MYUBRR );		//UART���W�X�^������
	
	//���荞�݋���
	sei();
}

static void USART_Init( unsigned short baud )
{
	UBRR0H	= (unsigned char)(baud>>8);	//�{�[���[�g���
	UBRR0L	= (unsigned char) baud;		//�{�[���[�g����
	UCSR0C	= (1<<USBS0) | (3<<UCSZ00);	//��~bit2bit�A�f�[�^�r�b�g��8bit
//	UCSRC	= (3<<UCSZ0);	//��~bit1bit�A�f�[�^�r�b�g��8bit
//	UCSRB	= (1<<RXEN) | (1<<TXEN);	//��M���A���M����
//	UCSR0B	= (1<<TXEN0);	//��M���A���M����
}
static void powerLed( void )
{
	PORTB ^= (1<<PB1);
}
