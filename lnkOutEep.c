#include <stdbool.h>
#include "lnkOutEep_inc.h"
#include "lnkOutEep.h"
#include "aplCtrl.h"
#include "drvEep.h"

static unsigned char	eepReq;

//********************************************************************************
// 初期化
//********************************************************************************
void initLnkOutEep( void )
{
	eepReq = false;
}
//********************************************************************************
// APLよりセット
//********************************************************************************
void setLnkOutEep( void )
{
	eepReq = true;
}
//********************************************************************************
// メイン処理
//********************************************************************************
void lnkOutEepMain( void )
{
	APL_CTRL_SET	*inAplCtrlSet;
	DRV_EEP_WRITE	outDrvEepWrite;
	unsigned char	i,j;
	unsigned char	sum;
	
	inAplCtrlSet = getAplCtrlSet();

	//書込み要求有
	if( eepReq == true ){
		eepReq = false;

		i=0;
		outDrvEepWrite.val[i++]	= inAplCtrlSet->palseSpeed;
		outDrvEepWrite.val[i++]	= inAplCtrlSet->palseRev;

		sum = 0;
		for( j=0 ; j<DRV_EEP_MAP_SUM ; j++ ){
			sum += outDrvEepWrite.val[j];
		}
		outDrvEepWrite.val[i++]	= sum;

		setDrvEep( &outDrvEepWrite );
	}

}
