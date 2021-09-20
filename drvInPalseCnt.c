
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //割り込みを使用するため

#include "drvInPalseCnt_inc.h"
#include "drvInPalseCnt.h"
#include "drvIntimer.h"
#include "hardware.h"

//Lnkが取得する
static DRV_IN_PALSE_CNT	drvInPulseCnt;

//********************************************************************************
// 初期化
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
// LINKが取得
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

	cli();	//割り込み禁止

	//車速
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_SPEED );

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		stopTimer( DRV_IN_TIMER_ID_SPEED );	//タイマ停止
	}

	//回転数
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_REV );
	
	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = 0;
		stopDrvInTimer( DRV_IN_TIMER_ID_REV );	//タイマ停止
	}

	sei();
	
}
//********************************************************************************
// ポート変化割り込み(車速
//********************************************************************************
void interPosEdgeSpeed( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_SPEED );

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = inDrvInTimer.cnt;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//オーバーフロー状態クリア、再開
	}else{
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_SPEED] = 123;
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );
	}

	sei();
}

//********************************************************************************
// ポート変化割り込み(回転数
//********************************************************************************
void interPosEdgeRev( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_REV );

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = inDrvInTimer.cnt;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cycCnt[DRV_IN_PALSE_CNT_NO_REV] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_REV );	//オーバーフロー状態クリア、再開
	}else{
		startDrvInTimer( DRV_IN_TIMER_ID_REV );
	}

	sei();
}

