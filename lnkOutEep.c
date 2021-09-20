#include <stdbool.h>
#include "lnkOutEep_inc.h"
#include "lnkOutEep.h"
#include "aplCtrl.h"
#include "drvEep.h"

static unsigned char	eepReq;

//********************************************************************************
// ������
//********************************************************************************
void initLnkOutEep( void )
{
	eepReq = false;
}
//********************************************************************************
// APL���Z�b�g
//********************************************************************************
void setLnkOutEep( void )
{
	eepReq = true;
}
//********************************************************************************
// ���C������
//********************************************************************************
void lnkOutEepMain( void )
{
	APL_CTRL_SET	*inAplCtrlSet;
	DRV_EEP_WRITE	outDrvEepWrite;
	unsigned char	i,j;
	unsigned char	sum;
	
	inAplCtrlSet = getAplCtrlSet();

	//�����ݗv���L
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
