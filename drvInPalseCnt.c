
//#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //割り込みを使用するため
#include <stdbool.h>

#include "hardware.h"
#include "drvIntimer.h"
#include "drvInPalseCnt_inc.h"
#include "drvInPalseCnt.h"



void posEdgeSpeed( void );
void posEdgeRev( void );


//Lnkが取得する
static DRV_IN_PALSE_CNT	drvInPulseCnt;

static bool		palseReciveFlag[DRV_IN_PALSE_CNT_NO_MAX];

//********************************************************************************
// 初期化
//********************************************************************************
void initDrvInPalseCnt( void )
{
	unsigned char	i;

	for( i=0 ; i<DRV_IN_PALSE_CNT_NO_MAX ; i++ ){
		drvInPulseCnt.cyc100ns[i] = 0;
		drvInPulseCnt.palseReciveFlag[i] = false;

		palseReciveFlag[i]	= false;
	}
	// 割り込みレジスタ設定はmain.cへ移動。理由は特にない。最適方法模索中。
}

//********************************************************************************
// LINKが取得
//********************************************************************************
DRV_IN_PALSE_CNT *getDrvInPalesCnt( void )
{
	for( uint8_t i=0 ; i<DRV_IN_PALSE_CNT_NO_MAX ; i++ ){
		palseReciveFlag[i]	= false;
	}
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
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED] = 0;		// 配列の添字が長い
		stopDrvInTimer( DRV_IN_TIMER_ID_SPEED );	//タイマ停止
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){		// パルス周期計測中でまだ次のパルスが来てない場合、現在までのカウントを入れる
		if(	palseReciveFlag[DRV_IN_PALSE_CNT_NO_SPEED] == false ){
			drvInPulseCnt.palseReciveFlag[DRV_IN_PALSE_CNT_NO_SPEED] = false;
			drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED]		 = inDrvInTimer.cnt100ns;
		}
	}
																	// 車速と回転数まとめられない？
	//回転数
	inDrvInTimer	= getDrvInTimerState( DRV_IN_TIMER_ID_REV );
	
	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV] = 0;
		stopDrvInTimer( DRV_IN_TIMER_ID_REV );	//タイマ停止
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		if(	palseReciveFlag[DRV_IN_PALSE_CNT_NO_REV] == false ){
			drvInPulseCnt.palseReciveFlag[DRV_IN_PALSE_CNT_NO_REV]	= false;
			drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV]			= inDrvInTimer.cnt100ns;
		}
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
	bool				modeSplit = false;									// パルスが来たからスプリットではない。
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_SPEED , modeSplit );

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_SPEED]	= inDrvInTimer.cnt100ns;
		palseReciveFlag[DRV_IN_PALSE_CNT_NO_SPEED]			= true;
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
	bool				modeSplit = false;
	inDrvInTimer	= getDrvInTimer( DRV_IN_TIMER_ID_REV , modeSplit);

	cli();	//割り込み禁止

	if( inDrvInTimer.state == DRV_IN_TIMER_STATE_START ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV]		= inDrvInTimer.cnt100ns;
		palseReciveFlag[DRV_IN_PALSE_CNT_NO_REV]			= true;
	}else if( inDrvInTimer.state == DRV_IN_TIMER_STATE_OVERFLOW ){
		drvInPulseCnt.cyc100ns[DRV_IN_PALSE_CNT_NO_REV] = 0;
		startDrvInTimer( DRV_IN_TIMER_ID_REV );	//オーバーフロー状態クリア、再開
	}else{
		startDrvInTimer( DRV_IN_TIMER_ID_REV );
	}

	sei();
}

