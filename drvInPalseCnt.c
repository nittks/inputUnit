
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //���荞�݂��g�p���邽��

#include "drvInPalseCnt_inc.h"
#include "drvInPalseCnt.h"
#include "drvIntimer.h"
#include "hardware.h"

//Lnk���擾����
static DRV_IN_PALSE_CNT	drvInPulseCnt;

//********************************************************************************
// ������
//********************************************************************************
void initDrvInPalseCnt( void )
{
	unsigned char	i;

	for( i=0 ; i<DRV_IN_PALSE_CNT_NO_MAX ; i++ ){
		drvInPulseCnt.cycCnt[i] = 0;
	}
	SET_EICRA;
	SET_EIMSK;

}

//********************************************************************************
// LINK���擾
//********************************************************************************
DRV_IN_PALSE_CNT *getDrvInPalesCnt( void )
{
	return( &drvInPulseCnt );
}

//********************************************************************************
// main
//********************************************************************************
void drvInPalseCntMain( void )
{
	
	DRV_IN_TIMER		inDrvInTimer;

	cli();	//���荞�݋֎~

	//�ԑ�
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_SPEED );

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		stopTimer( DRV_IN_TIMER_ID_SPEED );	//�^�C�}��~
	}

	//��]��
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_REV );
	
	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = 0;
		stopDrvInTimer( DRV_IN_TIMER_ID_REV );	//�^�C�}��~
	}

	sei();
	
}
//********************************************************************************
// �|�[�g�ω����荞��(�ԑ�
//********************************************************************************
void interPosEdgeSpeed( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_SPEED );

	cli();	//���荞�݋֎~

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = inDrvInTimer.cnt;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//�I�[�o�[�t���[��ԃN���A�A�ĊJ
	}else{
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 123;
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );
	}

	sei();
}

//********************************************************************************
// �|�[�g�ω����荞��(��]��
//********************************************************************************
void interPosEdgeRev( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_REV );

	cli();	//���荞�݋֎~

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = inDrvInTimer.cnt;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_REV );	//�I�[�o�[�t���[��ԃN���A�A�ĊJ
	}else{
		startDrvInTimer( DRV_IN_TIMER_ID_REV );
	}

	sei();
}

