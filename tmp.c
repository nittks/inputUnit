/*
 * 
 *
 * Created: 2016/05/06 8:25:08
 *  Author: sin
 */ 

/*
定期タスク
MPU		:ATmega3208
clock	:内蔵20MHz
*/

#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdbool.h>

#include "timer.h"
#include "drvInMain.h"
#include "drvOutMain.h"
#include "lnkInMain.h"
#include "lnkOutMain.h"
#include "aplMain.h"
#include "drvUart.h"
#include "main_inc.h"

#define INT_CNT_MAX	((unsigned char)125)	//8us*125=1ms毎割り込み

static uint16_t		waitBootTimeCntMs=0;
volatile static uint16_t	cnt=0;

static void mainTask( void );
static void waitLedBoot( void );
static void initReg( void );
static void initTaskTimer( void );
//********************************************************************************
// メイン
//********************************************************************************
int main(void)
{
	initMain();

	while(1)
	{
		//処理したらスリープ。タイマ割り込みで起きたら再度ループ開始
		mainTask();
//		SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
		set_sleep_mode(SLEEP_MODE_IDLE);
	}
}

//********************************************************************************
// 初期化
//********************************************************************************
void initMain( void )
{

	initReg();
	
	initDrvIn();
	initLnkIn();
	initApl();
	initLnkOut();
	initDrvOut();
	
	initTaskTimer();
	
/*		
	atmel_start_init();
		
	USART_0_init();
	USART_0_enable();
	USART_0_enable_tx();

	sei();

*/
	/* Replace with your application code */
/*
	while (1) {
		cnt++;
		if( USART_0_is_tx_ready() != 0 ){
			USART_0_write( 0x55 );
		}
*/		

//	for( i=0 ; i<TASK_NUM ; i++ ){
//		taskParameter[i] = taskParameterDefault[i];
//	}
}
static void mainTask( void )
{
	unsigned char	i;
	//PORTD.OUTTGL	= 0x40;
	//処理タスク実行時間チェック
	for( i=0; i<TASK_MAX ; i++){
		if( taskParameter[i].regist == true ){	//タスク有効

			//周期時間経過していたら処理
			if( taskParameter[i].currentTime <= 0 ){
				//リセット
				taskParameter[i].currentTime = taskParameter[i].cycleTime; 
				//タスク実行

				PORTA.OUT	|= 0x40;
				taskParameter[i].func();
				PORTA.OUT	&= (~0x40);
			}
		}
	}
}

//********************************************************************************
// タスク時間カウント
//********************************************************************************
void interTaskTime( void )
{
	unsigned char	i;

	cli();
	
//	PORTF.OUTTGL	= (PIN4_bm);
	
	TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;

	//処理タスク実行時間チェック
	for( i=0; i<TASK_MAX ; i++){
		if( taskParameter[i].regist == true ){	//登録有効タスクのみ

			//10msで1カウントダウン。
			if( taskParameter[i].currentTime > 0 ){
				taskParameter[i].currentTime--;
			}
		}
	}
	sei();
}
//********************************************************************************//
// タスク有効化
//********************************************************************************//
void enableTask( unsigned char taskNo )
{
	//タスク有効化
	taskParameter[taskNo].regist	= true;
	//周期セット
	taskParameter[taskNo].currentTime	= taskParameter[taskNo].cycleTime;
}
//********************************************************************************//
// タスク無効化
//********************************************************************************//
void disableTask( unsigned char taskNo )
{
	//タスク有効化
	taskParameter[taskNo].regist	= false;
}

//********************************************************************************
// レジスタ初期化
//********************************************************************************
static void initReg(void)
{
	cli();

	//クロック設定
	FUSE.OSCCFG			= FUSE_OSCCFG_FREQSEL_20MHZ;
	// 20MHz -> (分周無し) -> CLK_PER=20MHz
//	CLKCTRL.MCLKCTRLB	= CLKCTRL_PDIV_8X_gc | CLKCTRL_PEN_bm;
	CPU_CCP				= CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB	= 0;
	CLKCTRL.MCLKCTRLA	= CLKCTRL_CLKSEL_OSC20M_gc;
	
	
		//I/O設定
	PORTA.DIR	= 0x49;
	PORTC.DIR	= 0x00;
	PORTD.DIR	= 0x00;
	PORTF.DIR	= 0x00;
	
	PORTA.OUT	= 0x00;
	PORTC.OUT	= 0x00;
	PORTD.OUT	= 0x00;
	PORTF.OUT	= 0x00;
	

	PORTA.PIN0CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTA.PIN1CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTA.PIN2CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTA.PIN3CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTA.PIN4CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTA.PIN5CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTA.PIN6CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTA.PIN7CTRL	= (1<<PORT_PULLUPEN_bp) ;

	PORTC.PIN0CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTC.PIN1CTRL	= (0<<PORT_PULLUPEN_bp) ;
	PORTC.PIN2CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTC.PIN3CTRL	= (0<<PORT_PULLUPEN_bp) ;

	PORTD.PIN0CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN1CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN2CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN3CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN4CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN5CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN6CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTD.PIN7CTRL	= (1<<PORT_PULLUPEN_bp) ;

	PORTF.PIN0CTRL	= (0<<PORT_PULLUPEN_bp) | (PORT_ISC_RISING_gc) ;
	PORTF.PIN1CTRL	= (0<<PORT_PULLUPEN_bp) | (PORT_ISC_RISING_gc) ;
	PORTF.PIN2CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTF.PIN3CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTF.PIN4CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTF.PIN5CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTF.PIN6CTRL	= (1<<PORT_PULLUPEN_bp) ;
	PORTF.PIN7CTRL	= (1<<PORT_PULLUPEN_bp) ;
	
	//割り込み許可
	sei();
}
//********************************************************************************
// タスク時間管理用タイマ、レジスタ設定
//********************************************************************************
static void initTaskTimer( void )
{
	cli();
	
	//タイマ設定
	// 1msで割り込みしたい
	// CLK_PER=20MHz -> (16分周) -> 1250kHz(0.8us周期) -> 1250カウントで1ms
	TCA0.SINGLE.PER			= (F_CPU/F_PDIV/16)/1000;
	TCA0.SINGLE.CTRLESET	= TCA_SINGLE_DIR_UP_gc;
	TCA0.SINGLE.CTRLA		= TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;
	TCA0.SINGLE.CTRLB		= TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SINGLE.INTCTRL		= TCA_SINGLE_OVF_bm;

	sei();
}

//********************************************************************************
// 動作確認用LED点滅
//********************************************************************************
static void powerLed( void )
{
//	PORTB ^= (1<<PB1);
}
