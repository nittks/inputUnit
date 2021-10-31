
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //割り込みを使用するため
#include <string.h>
#include <stdbool.h>

#include "drvUart_inc.h"
#include "drvUart.h"
#include "hardware.h"
#include "timer.h"

static UART_STATE		uartState;
//----------------------------------------
// 送信
//----------------------------------------
static DRV_UART_TX		drvUartTx;
static unsigned char	txDataCnt;
static unsigned char	txCnt;
static unsigned char	txReqFlag;

//----------------------------------------
// 受信
//----------------------------------------
static DRV_UART_RX		drvUartRx;
static unsigned char	rxDataBuf[DRV_UART_RX_BUF_SIZE];			//受信中データを入れていくバッファ
static unsigned char	rxDataBufCnt;		//受信中データバッファ用カウンタ
static unsigned char	rxDataCnt;			//URATデータ長カウンタ
static unsigned char	rxDataLen;			//UARTフレームより取得したフレームレングス
static unsigned char	rxFlag;				//受信有無

//********************************************************************************//
// 初期化
//********************************************************************************//
void initDrvUart( void )
{
	unsigned char	i;

	//送信
	for( i=0 ; i<DRV_UART_RX_BUF_SIZE; i++ ){
		drvUartTx.txData[i]	= 0;
	}
	drvUartTx.txDataNum =	0;
	txDataCnt	= 0;
	txCnt		= 0;
	txReqFlag	= false;

	//受信
	for( i=0 ; i<DRV_UART_RX_BUF_SIZE ; i++ ){
		drvUartRx.rxData[i]	= 0;
	}
	drvUartRx.rxDataNum =	0;
	drvUartRx.rxFlag = false;
		
	rxDataCnt		= 0;
	rxDataLen		= 0;

	//送受信許可
	EN_UART_TX;
	EN_UART_RX;
//	UCSR0B	= (1<<TXEN0);

	//受信完了割込み許可
	uartState		= UART_STATE_STANDBY;
	EN_INTER_UART_RX_COMP;
	RS485_RX;
}

//********************************************************************************//
// 送信セット
//********************************************************************************//
void setDrvUartTx( DRV_UART_TX *inP )
{
	drvUartTx	= *inP;

	if( uartState == UART_STATE_STANDBY ){
		uartState = UART_STATE_TRANS;
		RS485_TX;						//RS485トランシーバ方向切替
		EN_INTER_UART_TX_REG_EMPTY;		//送信バッファ空割込み許可 
		DI_INTER_UART_RX_COMP;			//受信完了割込み禁止
	}
}

//********************************************************************************//
// 送信レジスタ空割込み
//********************************************************************************//
void interSetUartTxData(void)
{
	cli();	//割り込み禁止

	while( UART_REG_UDRE == UDRE_EMPTY ){	//送信レジスタ空の間回す
		UDR0 = drvUartTx.txData[txDataCnt];
		txDataCnt++;

		if( txDataCnt >= drvUartTx.txDataNum ){	//全データ送信済み

			txDataCnt = 0;
			DI_INTER_UART_TX_REG_EMPTY;		//送信レジスタ空割込み禁止
			EN_INTER_UART_TX_FIN;			//送信完了割込み許可
		}
	}
	sei();	//割込み許可
}

//********************************************************************************//
// 送信完了割込み
//********************************************************************************//
void interUartTxFin(void)
{
	cli();	//割り込み禁止

	uartState = UART_STATE_STANDBY;

	RS485_RX;						//RS485トランシーバは受信状態で待機
	EN_INTER_UART_RX_COMP;			//受信完了割込み許可

	sei();	//割込み許可
}


//********************************************************************************//
// 受信データ取得
//********************************************************************************//
DRV_UART_RX *getDrvUartRx( void )
{
	//受信有無フラグを公開変数へ入れ、クリアする
	drvUartRx.rxFlag = rxFlag;
	rxFlag = false;
	return( &drvUartRx );
}


//********************************************************************************//
// UART受信データ割り込み処理
//********************************************************************************//
void interGetUartRxData(void)
{
	unsigned char	rxBuf;
	unsigned char	timerCnt;

	cli();	//割り込み禁止
	while( UART_REG_RXC == RXC_IN_DATA){

		//レジスタよりデータ取得
		rxBuf = UDR0;
		
		//タイマオーバーフロー or フレーム間タイムアウト
		//フレームの最初(ID)から受信しなおす
		timerCnt	= getTimerCnt  ( TIMER_DRV_IN_UART_TIMEOUT );
		if( (timerCnt == TIMER_OVER_FLOW ) || (timerCnt > UART_FRAME_TIMEOUT) ){
			uartState = UART_STATE_STANDBY;	//待機中へリセット
			rxDataCnt=0;
			rxDataBufCnt=0;
		}else{
			//受信したので、タイムアウトタイマクリア
			clearTimer( TIMER_DRV_IN_UART_TIMEOUT );
		}
		//データチェック
		if( uartState == UART_STATE_STANDBY){
			//フレームID判定
			if(( rxDataCnt == UART_DATAPOS_ID ) &&		//ID位置
			( rxBuf == UART_ID_SETTING )				//ID一致
			){
				uartState = UART_STATE_RECEIVE;	//受信状態へ移行
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;
			}
		}else if( uartState == UART_STATE_RECEIVE){
			//データ長取得
			if( rxDataCnt == UART_DATAPOS_LENGTH ){
				rxDataLen = rxBuf;		//フレーム長記録
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;

			//通常受信
			}else{
				rxDataBuf[rxDataCnt] = rxBuf;
				rxDataCnt++;
					
				//受信完了
				if( rxDataCnt >= rxDataLen ){
					uartState = UART_STATE_STANDBY;
					//Lnk取得用配列へコピー
					memcpy( &drvUartRx.rxData[0] , &rxDataBuf[0] , rxDataCnt);
					rxFlag = true;		//受信有り
					rxDataCnt = 0;
				}
			}
		}else{
			//取り得ない
		}
	}
	sei();	//割込み許可

}


//********************************************************************************//
// UART
// ボーレート=9600bps(9.6b/ms
// 受信データ=5byte
// スタートbit=1,ストップbit=2bit,データ=8bit,パリティ=なし→1byte11bitデータ
// 5x11=55bit送信する
// 55bit/9.6bpms = 5.72ms →1フレーム辺り、最低6ms以上は必要
//********************************************************************************//
