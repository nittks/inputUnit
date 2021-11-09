
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drvInTimer_inc.h"
#include "drvInTimer.h"
#include "aplData.h"

static DRV_IN_TIMER		drvInTimer[DRV_IN_TIMER_NUM];

static TIMER_INS		timerIns[DRV_IN_TIMER_NUM];
static TIMER_STATE		timerRegState;		//マイコンタイマ状態

//************************************************************
//初期化
//************************************************************
void initDrvInTimer( void )
{
	unsigned char	i;

	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		drvInTimer[i].state	= DRV_IN_TIMER_STATE_STOP;
		drvInTimer[i].cnt	= 0;
		timerIns[i].cnt	= 0;
		timerIns[i].startCnt	= 0;
	}

	timerRegState = TIMER_STATE_STOP;
	
	//タイマ設定

	TCB0.CTRLA		= TCB_CLKSEL_CLKDIV1_gc;
	TCB0.CTRLB		= TCB_CNTMODE_INT_gc;
	TCB0.INTCTRL	= TCB_CAPT_bm;
	TCB0.CCMP		= TIMER_REG_MAX;

}

//************************************************************
//メイン処理
//************************************************************
void drvInTimerMain( void )
{
}

//************************************************************
// タイマスタート
//************************************************************
void startDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned short	inTimerReg;

	cli();
	//レジスタからタイマ値取得(誤差を少なくするため、最初に読取る
	inTimerReg	= GET_TIMER_CNT;

	//マイコンのタイマが止まっている時は、タイマ機能動作開始させる
	if( timerRegState == TIMER_STATE_STOP ){
		SET_TIMER_CNT( 0 );
		inTimerReg	= 0;
		EN_INTER_OVERFLOW_1A;
		START_TIMER_1A;
		timerRegState = TIMER_STATE_START;
	}
	//スタート＆クリア
	drvInTimer[timerId].state		= DRV_IN_TIMER_STATE_START;
	drvInTimer[timerId].cnt			= 0;

	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt	= inTimerReg;
	
	sei();
}

//************************************************************
// タイマクリア
//************************************************************
void clearDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	drvInTimer[timerId].cnt		= 0;
	
}
//************************************************************
// タイマストップ
//************************************************************
void stopDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned char	i;
	DRV_IN_TIMER_STATE		chkTimerState;

	cli();
	//全てのタイマが止まっていたら、マイコンのタイマを止める
	chkTimerState = DRV_IN_TIMER_STATE_STOP;
	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		if( drvInTimer[i].state	!= DRV_IN_TIMER_STATE_STOP ){
			chkTimerState = DRV_IN_TIMER_STATE_START;	//動いている
		}
	}
	if( (chkTimerState == DRV_IN_TIMER_STATE_STOP ) &&
		(timerRegState == TIMER_STATE_START)){
		STOP_TIMER_1A;
		DI_INTER_OVERFLOW_1A;
		SET_TIMER_CNT( 0 );
	}

	drvInTimer[timerId].state	= DRV_IN_TIMER_STATE_STOP;
	drvInTimer[timerId].cnt		= 0;
	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt	= 0;

	sei();
}
//************************************************************
// タイマ値取得
//************************************************************
DRV_IN_TIMER getDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned short	inTimerReg;
	
	cli();

	//レジスタからタイマ値取得
	inTimerReg	= GET_TIMER_CNT;

	//計測開始タイマカウントレジスタ値から現在の値を引き、経過時間を出す
	timerIns[timerId].cnt += (inTimerReg - timerIns[timerId].startCnt);

	//オーバーフロー(0km/h)検出
	if(	timerIns[timerId].cnt > TIMER_CNT_MAX[N1_04] ){
		timerIns[timerId].state	= DRV_IN_TIMER_STATE_OVERFLOW;
	}
	//公開用変数へコピー
	drvInTimer[timerId].cnt		= timerIns[timerId].cnt;
	//次の周期カウント用にクリア
	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt = inTimerReg;		//計測開始カウント値更新

	
	sei();

	//getした瞬間のデータが欲しいため、実態を返す
	//(ポインタだと使用する前に割り込みにより値が変化する可能性がある
	return( drvInTimer[timerId] );
}
//************************************************************
// タイマ状態のみ取得
//************************************************************
DRV_IN_TIMER getDrvInTimerState( DRV_IN_TIMER_ID timerId )
{
	return( drvInTimer[timerId] );
}
//************************************************************
// マイコンのタイマオーバーフロー
//************************************************************
void interDrvInTimerOverflow( void )
{
	unsigned char	i;
	
	cli();
	
	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		//計測開始タイマカウントレジスタ値から現在の値を引き、経過時間を出す
		timerIns[i].cnt += (TIMER_REG_MAX - timerIns[i].startCnt);
		timerIns[i].startCnt = 0;		//計測開始カウント値更新
	//オーバーフロー(0km/h)検出
		if(	timerIns[i].cnt > TIMER_CNT_MAX[N1_04] ){
			drvInTimer[i].state	= DRV_IN_TIMER_STATE_OVERFLOW;
//			drvInTimer[i].cnt		= 0;
//			stopTimer(i);
		}
	}
	
	sei();
}



