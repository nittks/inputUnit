#include <avr/io.h>
#include <stdbool.h>

#include "lnkInEep_inc.h"
#include "lnkInEep.h"

#include "hardware.h"
#include "aplData.h"
#include "drvEep.h"

static unsigned char	eepReadFlag;		//����EEP�f�[�^���f�ς݃t���O

//********************************************************************************
// ������
//********************************************************************************
void initLnkInEep( void )
{
	eepReadFlag = false;
}
//********************************************************************************
// ���C������
//********************************************************************************
void lnkInEepMain( void )
{
	DRV_EEP_READ	*inDrvEep;
	APL_DATA_EEP	aplDataEep;
	unsigned char	i;
	unsigned char	sum;

	//SW���̓h���C�o�f�[�^�擾
	inDrvEep = getDrvEep();

	if( eepReadFlag == false ){		//�����f
		if( inDrvEep->readState == DRV_EEP_READ_STATE_READED ){
			eepReadFlag = true;		//���f�ς�

			sum = 0;
			for( i=0 ; i<EEP_DATA_MAX-1 ; i++ ){	//SUM�������f�[�^
				sum += inDrvEep->val[i];
			}
			//SUM����
			if( sum == inDrvEep->val[DRV_EEP_MAP_SUM] ){
				i=0;
				aplDataEep.read				= APL_DATA_EEP_STATE_READED;
				aplDataEep.palseSpeed		= inDrvEep->val[i++];
				aplDataEep.palseRev			= inDrvEep->val[i++];
				
				//�͈̓u���b�N
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
