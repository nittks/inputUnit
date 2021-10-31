#include <avr/io.h>
#include <stdbool.h>

#include "lnkInCom_inc.h"
#include "lnkInCom.h"

#include "lnkInPalseCnt.h"
#include "aplData.h"
#include "drvUart.h"

//初期化
void initLnkInCom( void )
{
}
//メイン処理
void lnkInComMain( void )
{
	DRV_UART_RX		*inDrvUartRx;
	APL_DATA_COM	outAplDataCom;
	LNK_IN_PALSE_CNT	outLnkInPalseCnt;
	unsigned char	i;
	unsigned char	sum;
	
	inDrvUartRx = getDrvUartRx();

	if( inDrvUartRx->rxFlag == true ){

		sum=0;
		for( i=0 ; i<inDrvUartRx->rxDataNum-1 ; i++ ){
			sum += inDrvUartRx->rxData[i];
		}
		
		if( sum != inDrvUartRx->rxData[inDrvUartRx->rxDataNum-1]){
			//SUMエラー。処理無し
		}else{
			outAplDataCom.chReq		= true;
			outAplDataCom.palseSpeed	= inDrvUartRx->rxData[UART_NO_PALSE_SET] & 0x0F;
			outAplDataCom.palseRev		= inDrvUartRx->rxData[UART_NO_PALSE_SET] >> 4;

			setAplDataCom( &outAplDataCom );
		
			outLnkInPalseCnt.palseSpeed	= outAplDataCom.palseSpeed;
			outLnkInPalseCnt.palseRev	= outAplDataCom.palseRev;
//			setLnkInPalseCnt( &outLnkInPalseCnt );
		}
	}
}
