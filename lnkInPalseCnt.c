#include <avr/io.h>

#include "lnkInPalseCnt_inc.h"
#include "lnkInPalseCnt.h"

#include "drvInPalseCnt.h"
#include "aplData.h"

LNK_IN_PALSE_CNT	lnkInPalseCnt;

//********************************************************************************
// 初期化
//********************************************************************************
void initLnkInPalseCnt( void )
{
	lnkInPalseCnt.palseSpeed	= INIT_PALSE_SPEED;
	lnkInPalseCnt.palseRev		= INIT_PALSE_REV; 
}
//********************************************************************************
// 設定値セット
//********************************************************************************
void setLnkInPalseCnt( LNK_IN_PALSE_CNT *inP )
{
	lnkInPalseCnt.palseSpeed	= SETTING_PALSE_SPEED[inP->palseSpeed];
	lnkInPalseCnt.palseRev		= SETTING_PALSE_REV[inP->palseRev];
}
//********************************************************************************
// メイン処理
//********************************************************************************
void lnkInPalseCntMain( void )
{
	
	DRV_IN_PALSE_CNT	*drvInPalseCnt;
	APL_DATA_PALSE		aplDataPalse;
	unsigned long		tmpCalcSpeed,tmpCalcRev;
	volatile unsigned long		tmp;
//	volatile unsigned long		tmpData[20];
	
	drvInPalseCnt	= getDrvInPalesCnt();

	//車速計算
	if( drvInPalseCnt->cyc100ns[NO_SPEED] == 0 ){
		aplDataPalse.speed = 0;		//未計測
	}else{
		//周期振れにより、車速の境界を跨くので四捨五入し振れを抑える
//		tmpCalcSpeed	= ROUND_DIGIT*((unsigned long)(60*60)*1*TIME_US/drvInPalseCnt->cyc100ns[NO_SPEED])/(N1*N2);
		tmpCalcSpeed	= ROUND_DIGIT*((unsigned long)(60*60)*1*TIME_US/drvInPalseCnt->cyc100ns[NO_SPEED]/100)/(lnkInPalseCnt.palseSpeed*N2);
		tmpCalcSpeed	= (tmpCalcSpeed + ROUND5) /ROUND_DIGIT;
		aplDataPalse.speed	= tmpCalcSpeed;
	}
	
	//回転数計算
	if( drvInPalseCnt->cyc100ns[NO_REV] == 0 ){
		aplDataPalse.rev = 0;
	}else{
//		aplData.rev		= (unsigned long)TIME_US*60/drvInPalseCnt->cyc100ns[NO_REV]/C; 
		tmp				= ROUND_DIGIT*((unsigned long)TIME_US*60/drvInPalseCnt->cyc100ns[NO_REV])/C; 
		tmpCalcRev		= ROUND_DIGIT*((unsigned long)TIME_US*60/drvInPalseCnt->cyc100ns[NO_REV]);
		tmpCalcRev		= ((tmpCalcRev*10) / (10*lnkInPalseCnt.palseRev/R_SP));
		tmpCalcRev		= (tmpCalcRev + ROUND5) /ROUND_DIGIT;
		aplDataPalse.rev		= tmpCalcRev;
	}
	setAplDataPalse( &aplDataPalse );
}
