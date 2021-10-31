#include <avr/io.h>
#include <stdbool.h>

#include "lnkInEep_inc.h"
#include "lnkInEep.h"

#include "hardware.h"
#include "aplData.h"
#include "drvEep.h"

static unsigned char	eepReadFlag;		//初回EEPデータ反映済みフラグ

//********************************************************************************
// 初期化
//********************************************************************************
void initLnkInEep( void )
{
	eepReadFlag = false;
}
//********************************************************************************
// メイン処理
//********************************************************************************
void lnkInEepMain( void )
{
	DRV_EEP_READ	*inDrvEep;
	APL_DATA_EEP	aplDataEep;
	unsigned char	i;
	unsigned char	sum;

	//SW入力ドライバデータ取得
	inDrvEep = getDrvEep();

	if( eepReadFlag == false ){		//未反映
		if( inDrvEep->readState == DRV_EEP_READ_STATE_READED ){
			eepReadFlag = true;		//反映済み

			sum = 0;
			for( i=0 ; i<EEP_DATA_MAX-1 ; i++ ){	//SUMを除くデータ
				sum += inDrvEep->val[i];
			}
			//SUM正常
			if( sum == inDrvEep->val[DRV_EEP_MAP_SUM] ){
				i=0;
				aplDataEep.read				= APL_DATA_EEP_STATE_READED;
				aplDataEep.palseSpeed		= inDrvEep->val[i++];
				aplDataEep.palseRev			= inDrvEep->val[i++];
				
				//範囲ブロック
				if( (aplDataEep.palseSpeed < PALSE_SPEED_MIN ) ||
				(PALSE_SPEED_MAX < aplDataEep.palseSpeed) )
				{
					aplDataEep.palseSpeed = PALSE_SPEED_DEFAULT;
				}

				if( (aplDataEep.palseRev < PALSE_REV_MIN ) ||
				(PALSE_REV_MAX < aplDataEep.palseRev) )
				{
					aplDataEep.palseRev = PALSE_REV_DEFAULT;
				}

				
			}else{
				aplDataEep.read				= APL_DATA_EEP_STATE_SUMERROR;
				aplDataEep.palseSpeed		= PALSE_SPEED_DEFAULT;
				aplDataEep.palseRev			= PALSE_REV_DEFAULT;
			}
			setAplDataEep( &aplDataEep );
		}
	}
}
