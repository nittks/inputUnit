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
// 初期化
//********************************************************************************
void initAplCtrl( void )
{
	aplCtrl.state		= APL_CTRL_STATE_BOOT;
}
//********************************************************************************
// 制御状態取得
//********************************************************************************
APL_CTRL *getAplCtrl( void )
{
	return( &aplCtrl );
}
//********************************************************************************
// パルス設定値取得
//********************************************************************************
APL_CTRL_SET *getAplCtrlSet( void )
{
	return( &aplCtrlSet );
}
//********************************************************************************
// メイン処理
//********************************************************************************
void aplCtrlMain( void )
{

	//状態判定、状態遷移
	stateJudge();

	//各状態時処理
	proc();
}
//********************************************************************************
// 状態判定、状態遷移
//********************************************************************************
static void stateJudge( void )
{
	APL_DATA_EEP	*inAplDataEep;

	inAplDataEep	= getAplDataEep();

	switch( aplCtrl.state ){
	//****************************************
	// 初回起動
	//****************************************
	case APL_CTRL_STATE_BOOT:		//起動初回
		if( inAplDataEep->read != APL_DATA_EEP_STATE_UNREAD ){		//EEPROM読み込み済み
			apryEep();		//初回はEEPROMのデータを適用する
			aplCtrl.state = APL_CTRL_STATE_NOMARL;
		}
		break;

	//****************************************
	// 通常
	//****************************************
	case APL_CTRL_STATE_NOMARL:		//通常
		//無処理
		break;

	}
}
//********************************************************************************
// 各状態時処理
//********************************************************************************
static void proc( void )
{
	APL_DATA_COM		*inAplDataCom;
	LNK_IN_PALSE_CNT	outLnkInPalseCnt;

	inAplDataCom	= getAplDataCom();

	switch( aplCtrl.state ){
	case APL_CTRL_STATE_BOOT:		//起動初回
		//無処理
		break;

	case APL_CTRL_STATE_NOMARL:		//通常
		//設定変更要求有り
		if( inAplDataCom->chReq == true ){
			aplCtrlSet.palseSpeed	= inAplDataCom->palseSpeed;
			aplCtrlSet.palseRev		= inAplDataCom->palseRev;
			setLnkOutEep();		//EEPROM書込み要求
			outLnkInPalseCnt.palseSpeed	= inAplDataCom->palseSpeed; 
			outLnkInPalseCnt.palseRev	= inAplDataCom->palseRev; 
			setLnkInPalseCnt( &outLnkInPalseCnt );		//車速、回転数計算用定数変更
		}
		break;
	}
}
//********************************************************************************
// EEPROMデータ適用
//********************************************************************************
static void apryEep( void )
{
	APL_DATA_EEP	*inAplDataEep;

	inAplDataEep	= getAplDataEep();

	if( inAplDataEep->read == APL_DATA_EEP_STATE_READED){
		//読込済みなら反映
		aplCtrlSet.palseSpeed	= inAplDataEep->palseSpeed;
		aplCtrlSet.palseRev		= inAplDataEep->palseRev;
	}else if( inAplDataEep->read == APL_DATA_EEP_STATE_SUMERROR){
		//SUMエラー時はデフォルト値読込
		aplCtrlSet.palseSpeed	= eepDefault[PALSE_SPEED];
		aplCtrlSet.palseRev		= eepDefault[PALSE_REV];
		setLnkOutEep();	//EEPROM書込み要求
	}
}
