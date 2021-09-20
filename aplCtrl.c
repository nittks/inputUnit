#include <avr/io.h>
#include <stdbool.h>
#include "aplCtrl_inc.h"
#include "aplCtrl.h"
#include "aplData.h"
#include "lnkOutEep.h"
#include "lnkOutCom.h"
#include "lnkInPalseCnt.h"
#include "hardware.h"

static APL_CTRL			aplCtrl;
static APL_CTRL_SET		aplCtrlSet;

static void stateJudge( void );
static void proc( void );
static void apryEep( void );
//********************************************************************************
// ������
//********************************************************************************
void initAplCtrl( void )
{
	aplCtrl.state		= APL_CTRL_STATE_BOOT;
}
//********************************************************************************
// �����Ԏ擾
//********************************************************************************
APL_CTRL *getAplCtrl( void )
{
	return( &aplCtrl );
}
//********************************************************************************
// �p���X�ݒ�l�擾
//********************************************************************************
APL_CTRL_SET *getAplCtrlSet( void )
{
	return( &aplCtrlSet );
}
//********************************************************************************
// ���C������
//********************************************************************************
void aplCtrlMain( void )
{

	//��Ԕ���A��ԑJ��
	stateJudge();

	//�e��Ԏ�����
	proc();
}
//********************************************************************************
// ��Ԕ���A��ԑJ��
//********************************************************************************
static void stateJudge( void )
{
	APL_DATA_EEP	*inAplDataEep;

	inAplDataEep	= getAplDataEep();

	switch( aplCtrl.state ){
	//****************************************
	// ����N��
	//****************************************
	case APL_CTRL_STATE_BOOT:		//�N������
		if( inAplDataEep->read != APL_DATA_EEP_STATE_UNREAD ){		//EEPROM�ǂݍ��ݍς�
			apryEep();		//�����EEPROM�̃f�[�^��K�p����
			aplCtrl.state = APL_CTRL_STATE_NOMARL;
		}
		break;

	//****************************************
	// �ʏ�
	//****************************************
	case APL_CTRL_STATE_NOMARL:		//�ʏ�
		//������
		break;

	}
}
//********************************************************************************
// �e��Ԏ�����
//********************************************************************************
static void proc( void )
{
	APL_DATA_COM		*inAplDataCom;
	LNK_IN_PALSE_CNT	outLnkInPalseCnt;

	inAplDataCom	= getAplDataCom();

	switch( aplCtrl.state ){
	case APL_CTRL_STATE_BOOT:		//�N������
		//������
		break;

	case APL_CTRL_STATE_NOMARL:		//�ʏ�
		//�ݒ�ύX�v���L��
		if( inAplDataCom->chReq == true ){
			aplCtrlSet.palseSpeed	= inAplDataCom->palseSpeed;
			aplCtrlSet.palseRev		= inAplDataCom->palseRev;
			setLnkOutEep();		//EEPROM�����ݗv��
			outLnkInPalseCnt.palseSpeed	= inAplDataCom->palseSpeed; 
			outLnkInPalseCnt.palseRev	= inAplDataCom->palseRev; 
			setLnkInPalseCnt( &outLnkInPalseCnt );		//�ԑ��A��]���v�Z�p�萔�ύX
		}
		break;
	}
}
//********************************************************************************
// EEPROM�f�[�^�K�p
//********************************************************************************
static void apryEep( void )
{
	APL_DATA_EEP	*inAplDataEep;

	inAplDataEep	= getAplDataEep();

	if( inAplDataEep->read == APL_DATA_EEP_STATE_READED){
		//�Ǎ��ς݂Ȃ甽�f
		aplCtrlSet.palseSpeed	= inAplDataEep->palseSpeed;
		aplCtrlSet.palseRev		= inAplDataEep->palseRev;
	}else if( inAplDataEep->read == APL_DATA_EEP_STATE_SUMERROR){
		//SUM�G���[���̓f�t�H���g�l�Ǎ�
		aplCtrlSet.palseSpeed	= eepDefault[PALSE_SPEED];
		aplCtrlSet.palseRev		= eepDefault[PALSE_REV];
		setLnkOutEep();	//EEPROM�����ݗv��
	}
}
