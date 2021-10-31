#include <stdbool.h>
#include "aplData.h"
#include "aplData_inc.h"


static APL_DATA_PALSE	aplDataPalse;
static APL_DATA_CAR_SW	aplDataCarSw;
static APL_DATA_EEP		aplDataEep;
static APL_DATA_COM		aplDataCom;


//********************************************************************************
// 初期化
//********************************************************************************
void initAplData( void )
{
	aplDataPalse.speed	= 0;
	aplDataPalse.rev	= 0;

	//車体信号
	aplDataCarSw.ig		= APL_DATA_SW_OFF;
	aplDataCarSw.ill	= APL_DATA_SW_OFF;
	aplDataCarSw.vtc	= APL_DATA_SW_OFF;
	aplDataCarSw.test	= APL_DATA_SW_OFF;		//基板SW

	//EEPROM
	aplDataEep.read			= APL_DATA_EEP_STATE_UNREAD;
	aplDataEep.palseSpeed	= 4;
	aplDataEep.palseRev		= 4;

	//通信
	aplDataCom.chReq		= false;
	aplDataCom.palseSpeed	= 0;
	aplDataCom.palseRev		= 0;

}
//********************************************************************************
// メイン処理
//********************************************************************************
void aplDataMain( void )
{
	//無し
}
//********************************************************************************
// パルス情報
//********************************************************************************
void setAplDataPalse( APL_DATA_PALSE * inData )
{
	aplDataPalse.speed	= inData->speed;
	aplDataPalse.rev	= inData->rev;
}
//********************************************************************************
// 車体スイッチ
//********************************************************************************
void setAplDataCarSw( APL_DATA_CAR_SW * inData )
{
	aplDataCarSw.ig		= inData->ig;
	aplDataCarSw.ill	= inData->ill;
	aplDataCarSw.vtc	= inData->vtc;
	aplDataCarSw.test	= inData->test;
}
//********************************************************************************
// 通信
//********************************************************************************
void setAplDataCom( APL_DATA_COM * inData )
{
	aplDataCom.chReq		= inData->chReq;
	aplDataCom.palseSpeed	= inData->palseSpeed;
	aplDataCom.palseRev		= inData->palseRev;
}
//********************************************************************************
// EEPROM
//********************************************************************************
void setAplDataEep( APL_DATA_EEP * inData )
{
	aplDataEep.read			= inData->read;
	aplDataEep.palseSpeed	= inData->palseSpeed;
	aplDataEep.palseRev		= inData->palseRev;
}

//********************************************************************************
// パルス情報
//********************************************************************************
APL_DATA_PALSE *getAplDataPalse( void )
{
	return( &aplDataPalse );
}
//********************************************************************************
// 車体スイッチ
//********************************************************************************
APL_DATA_CAR_SW *getAplDataCarSw( void )
{
	return ( &aplDataCarSw );
}
//********************************************************************************
// 通信
//********************************************************************************
APL_DATA_COM *getAplDataCom( void )
{
	return( &aplDataCom );
}
//********************************************************************************
// EEPROM
//********************************************************************************
APL_DATA_EEP *getAplDataEep( void )
{
	return( &aplDataEep );
}
