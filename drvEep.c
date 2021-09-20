
#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdbool.h>

#include "drvEep_inc.h"
#include "drvEep.h"
#include "hardware.h"

//�o�̓f�[�^
static DRV_EEP_READ		drvEepRead;
static DRV_EEP_WRITE	drvEepWrite;
static unsigned char	eepAddrCnt;

static unsigned char	eepCache[DRV_EEP_MAP_MAX];
static EEP_STATE		state;

static void eepRead( void );
static void eepWrite( void );
//********************************************************************************//
// ������
//********************************************************************************//
void initDrvEep( void )
{
	unsigned char		i;

	drvEepRead.readState = true;
	for( i=0; i<DRV_EEP_MAP_MAX ; i++ ){
		drvEepRead.val[i] = 50;
	}

	drvEepRead.readState = DRV_EEP_READ_STATE_UNREAD;	//���ǂݍ���
	state = EEP_STATE_BOOT;

	//�N�����Ǎ�
	eepAddrCnt = 0;
	eepRead();

}

//********************************************************************************//
// �f�[�^�擾
//********************************************************************************//
DRV_EEP_READ *getDrvEep( void )
{
	return( &drvEepRead );
}
//********************************************************************************//
// �f�[�^������
//********************************************************************************//
unsigned char setDrvEep( DRV_EEP_WRITE *inP )
{
	unsigned char		ret;

	if( state == EEP_STATE_READY ){
		state = EEP_STATE_WRITE;
		memcpy( drvEepWrite.val , inP->val , DRV_EEP_MAP_MAX );
		eepAddrCnt=0;
		eepWrite();

		ret = true;
	}else{
		ret = false;
	}

	return( ret );
}
//********************************************************************************//
// EEPROM�A�N�Z�X�\���荞��
//********************************************************************************//
void interEepRedy( void )
{
	if( state == EEP_STATE_READ ){		//�ǂݍ��ݓr��
		eepRead();
	}else if( state == EEP_STATE_WRITE ){	//�����ݓr��
		eepWrite();
	}
}
//********************************************************************************//
// �ǂݍ���
//********************************************************************************//
static void eepRead( void )
{
	//EEPROM�A�N�Z�X�\�ȊԁA�Ǎ�������B�A�N�Z�X�s��break����
	while( 1 ){
		if( eeprom_is_ready() == EEP_READY_FAIL ){
			//EEPROM�A�N�Z�X�s�̏ꍇ������
			state = EEP_STATE_READ;
			INT_EEP_ENABLE;		//EEPROM�A�N�Z�X�\�����݋���
			break;
		}else{
			eepCache[ eepAddrCnt ] =  eeprom_read_byte( (uint8_t *)eepAddrCnt );
			if( eepAddrCnt < DRV_EEP_MAP_MAX ){
				eepAddrCnt++;
			}else{
				//�S�f�[�^�ǂݍ��݊�
				state = EEP_STATE_READY;
				drvEepRead.readState = DRV_EEP_READ_STATE_READED;
				INT_EEP_DISABLE;		//EEPROM�A�N�Z�X�\�����݋֎~
				memcpy( drvEepRead.val ,eepCache , DRV_EEP_MAP_MAX );	//LNK�Ǎ��p�ϐ��փR�s�[

				break;
			}
		}
	}
}
//********************************************************************************//
// ������
//********************************************************************************//
static void eepWrite( void )
{
	
	//EEPROM�A�N�Z�X�\�ȊԁA�Ǎ�������B�A�N�Z�X�s��break����
	while( 1 ){
		if( eeprom_is_ready() == EEP_READY_FAIL ){
			//EEPROM�A�N�Z�X�s�̏ꍇ������
			state = EEP_STATE_WRITE;
			INT_EEP_ENABLE;		//EEPROM�A�N�Z�X�\�����݋���
			break;
		}else{
			//�l���قȂ��Ă����珑����
			if( drvEepWrite.val[ eepAddrCnt ] != eepCache[ eepAddrCnt ] ){
				eeprom_write_byte( (uint8_t*)eepAddrCnt ,drvEepWrite.val[eepAddrCnt] );
				eepCache[ eepAddrCnt ] =  drvEepWrite.val[ eepAddrCnt ];
			}

			if( eepAddrCnt < DRV_EEP_MAP_MAX-1 ){
				eepAddrCnt++;
			}else{
				//�S�f�[�^�����݊�
				state = EEP_STATE_READY;
				INT_EEP_DISABLE;		//EEPROM�A�N�Z�X�\�����݋֎~
				break;
			}
		}
	}
}
