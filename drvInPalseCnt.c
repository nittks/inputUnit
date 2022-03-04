
//#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //割り込みを使用するため

#include "hardware.h"
#include "drvIntimer.h"
#include "drvInPalseCnt_inc.h"
#include "drvInPalseCnt.h"



void posEdgeSpeed( void );
void posEdgeRev( void );


//Lnkが取得する
static DRV_IN_PALSE_CNT	drvInPulseCnt;

//********************************************************************************
// 初期化
//********************************************************************************
void initDrvInPalseCnt( void )
{
	unsigned char	i;

	for( i=0 ; i<DRV_IN_PALSE_CNT_NO_MAX ; i++ ){
		drvInPulseCnt.cyc100ns[i] = 0;
	}
	// 割り込みレジスタ設定はmain.cへ移動。理由は特にない。最適方法模索中。
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
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		stopDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//タイマ停止
	}

	//回転数
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_REV );
	
	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV] = 0;
		stopDrvInTimer( DRV_IN_TIMER_ID_REV );	//タイマ停止
	}

	sei();
	
}
//********************************************************************************
// ポート変化割り込み
//********************************************************************************
void interPortF( void )
{
	cli();
	
	//同時に立つことは無いはずだけど、一応別ifで記述
	
	if( PORTF.INTFLAGS & PORT_SPEED ){
		PORTF.INTFLAGS	=	PORT_SPEED;
		posEdgeSpeed();
	}
	
	if( PORTF.INTFLAGS & PORT_REV ){
		PORTF.INTFLAGS	=	PORT_REV;
		posEdgeRev();
	}
	
	sei();
}
//********************************************************************************
// 車速パルスカウント
//********************************************************************************
void posEdgeSpeed( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_SPEED );

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED] = inDrvInTimer.cnt100ns;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//オーバーフロー状態クリア、再開
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_STOP ){
		startDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//開始
	}

	sei();
}

//********************************************************************************
// 回転数パルスカウント
//********************************************************************************
void posEdgeRev( void )
{
	DRV_IN_TIMER		inDrvInTimer;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_REV );

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV] = inDrvInTimer.cnt100ns;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_REV );	//オーバーフロー状態クリア、再開
	}else{
		startDrvInTimer( DRV_IN_TIMER_ID_REV );
	}

	sei();
}

