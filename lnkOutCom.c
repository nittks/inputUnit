
#include "hardware.h"
#include "lnkOutCom_inc.h"
#include "lnkOutCom.h"
#include "drvUart.h"
#include "aplComData.h"

#include "drvInPalseCnt.h"

//初期化
void initLnkOutCom( void )
{
}
//メイン処理
void lnkOutComMain( void )
{
	APL_COM_DATA		*inAplComData=0;
	DRV_UART_TX			outDrvUartTx;
	unsigned char		i,j;
	unsigned char		sum;
	static unsigned char	timercnt=0;

	inAplComData = getAplComData();

	//通信UART出力データセット
	i=0;
	outDrvUartTx.txData[i++]	= UART_TX_ID;
	outDrvUartTx.txData[i++]	= UART_TX_LENGTH;
	outDrvUartTx.txData[i++]	= inAplComData->speed;
	outDrvUartTx.txData[i++]	= inAplComData->rev >> 8;		//上位
	outDrvUartTx.txData[i++]	= inAplComData->rev & 0x00FF;	//下位
	outDrvUartTx.txData[i++]	= (inAplComData->palseRev << 4 ) | (inAplComData->palseSpeed&0x0F) ;
	outDrvUartTx.txData[i++]	=	(inAplComData->vtc << 3) | 
									(inAplComData->ill << 2) |
									(inAplComData->acc << 1) |
									(inAplComData->ig);
	
	sum = 0;
	for( j=0 ; j<UART_TX_LENGTH-1 ; j++ ){
		sum += outDrvUartTx.txData[j];
	}
	outDrvUartTx.txData[i]	= sum;
	
	//データ数
	outDrvUartTx.txDataNum	= UART_TX_LENGTH;

	if( timercnt >= 3 ){
		setDrvUartTx( &outDrvUartTx );
		timercnt=0;
	}else{
		timercnt++;
	}

}

