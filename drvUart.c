
#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //割り込みを使用するため
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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

static int8_t USART_0_init();
static bool USART_0_is_tx_ready();
static void USART_0_write(const uint8_t data);
//********************************************************************************//
// 初期化
//********************************************************************************//
void initDrvUart( void )
{
	unsigned char	i;
	
	cli();
	
	USART_0_init();	
	
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
	rxFlag			= false;


	//送受信許可
//	EN_UART_TX;
//	EN_UART_RX;
//	UCSR0B	= (1<<TXEN0);

	//受信完了割込み許可
	uartState		= UART_STATE_STANDBY;
	EN_INTER_UART_RX_COMP;
	
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
	
	sei();
}
static int8_t USART_0_init()
{
	/* 周波数誤差保証 参照元：mega3209.pdf 日本語版 P.48,158 */
	/* 工場で格納された周波数誤差でのボーレート補償 */
	/* 自動ボーレート(同期領域)なしでの非同期通信 */
	
volatile	int8_t sigrow_val = SIGROW.OSC20ERR5V; // 符号付き誤差取得
volatile	int32_t baud_reg_val = (uint16_t)USART0_BAUD_RATE(9600);
	
	assert (baud_reg_val >= 0x4A); // 負の最大比較で正当な最小BAUDレジスタ値を確認
	baud_reg_val *= (1024 + sigrow_val); // (分解能+誤差)で乗算
	baud_reg_val /= 1024; // 分解能で除算
	USART0.BAUD = (int16_t) baud_reg_val; // 補正したボーレート設定

	USART0.CTRLA = 0
//			  0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
//			 | 1 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
//			 | 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
			 | USART_RS485_EXT_gc /* RS485 Mode disabled */
			 | 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
//			 | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
//			 | 1 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */
;
	USART0.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
//	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
//	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
			 | USART_CHSIZE_8BIT_gc /* Character size: 8 bit */
			 | USART_PMODE_DISABLED_gc /* No Parity */
			 | USART_SBMODE_2BIT_gc; /* 2 stop bit */

	// USART0.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART0.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART0.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART0.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */

	return 0;
}

//********************************************************************************//
// 送信セット
//********************************************************************************//
void setDrvUartTx( DRV_UART_TX *inP )
{
	cli();
	drvUartTx	= *inP;
		
	if( uartState == UART_STATE_STANDBY ){
		uartState = UART_STATE_TRANS;
		EN_INTER_UART_TX_REG_EMPTY;		//送信バッファ空割込み許可 
 		DI_INTER_UART_RX_COMP;			//受信完了割込み禁止
	}
/*
	if( USART_0_is_tx_ready() != 0 ){
		USART_0_write( 0x55 );
	}
*/
	sei();
}

//********************************************************************************//
// 受信完了→ウェイト後、送信モードへ移行
//********************************************************************************//
void drvUartChangeTx( void )
{
	cli();
	disableTask( TASK_UART_CHANGE_TX );		//本関数の起動無効化依頼
	uartState = UART_STATE_TRANS;
	
	EN_INTER_UART_TX_REG_EMPTY;		//送信バッファ空割込み許可 
	DI_INTER_UART_RX_COMP;			//受信完了割込み禁止
	sei();
}
													
//********************************************************************************//
// 送信レジスタ空割込み
//********************************************************************************//
void interSetUartTxData(void)
{
	cli();	//割り込み禁止
	
	while( UART_DATA_REG_EMP_FLG == DREIF_EMPTY ){	//送信レジスタ空の間回す
//		USART0.TXDATAL = drvUartTx.txData[txDataCnt];
		USART_0_write( drvUartTx.txData[txDataCnt] );

		txDataCnt++;

		if( txDataCnt >= drvUartTx.txDataNum ){	//全データ送信済み

			txDataCnt = 0;
			DI_INTER_UART_TX_REG_EMPTY;		//送信レジスタ空割込み禁止
			EN_INTER_UART_TX_FIN;			//送信完了割込み許可
			break;
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
	
	USART0.STATUS |= (USART_TXCIF_bm);

	uartState = UART_STATE_STANDBY;
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
	while( UART_REG_RXIC == RXC_IN_DATA){

		//レジスタよりデータ取得
		rxBuf = USART0.RXDATAL;
		
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
static bool USART_0_is_tx_ready()
{
	return (USART0.STATUS & USART_DREIF_bm);
}

static void USART_0_write(const uint8_t data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	;
	USART0.TXDATAL = data;
}

//********************************************************************************//
// UART
// ボーレート=9600bps(9.6b/ms
// 受信データ=5byte
// スタートbit=1,ストップbit=2bit,データ=8bit,パリティ=なし→1byte11bitデータ
// 5x11=55bit送信する
// 55bit/9.6bpms = 5.72ms →1フレーム辺り、最低6ms以上は必要
//********************************************************************************//
