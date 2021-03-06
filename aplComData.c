
#include <avr/io.h>
#include "aplComData_inc.h"
#include "aplComData.h"
#include "aplData.h"
#include "aplCtrl.h"

static APL_COM_DATA		aplComData;
static uint16_t			speedBuff[SPEED_BUF_NUM];
static uint8_t			speedBuffCnt;

static unsigned char	testCycSpeed;
static unsigned char	testCycRev;
static unsigned char	testSpeed;
static unsigned short	testRev;
static TEST_STATE		testStateSpeed	= TEST_STATE_UP;
static TEST_STATE		testStateRev;
static volatile uint8_t	debugSpeed = 123;
static uint8_t speedStabilize( uint16_t speed );
static unsigned char makeTestDataSpeed( void );
static unsigned short makeTestDataRev( void );
//********************************************************************************
// 初期化
//********************************************************************************
void initAplComData( void )
{
	aplComData.speed		= 0;
	aplComData.rev			= 0;
	aplComData.palseSpeed	= 0;
	aplComData.palseRev		= 0;
	aplComData.ig			= 0;
	aplComData.ill			= 0;
	aplComData.vtc			= 0;

	for( uint8_t i=0 ; i<SPEED_BUF_NUM ; i++ ){
		speedBuff[i]	= 0;
	}
	speedBuffCnt	= 0;
}
//********************************************************************************
// メイン処理
//********************************************************************************
void aplComDataMain( void )
{
	APL_DATA_PALSE		*inAplDataPalse;
	APL_DATA_CAR_SW		*inAplDataCarSw;
	APL_CTRL_SET		*inAplCtrlSet;

	//aplDataよりデータ取得
	inAplDataPalse	= getAplDataPalse();
	inAplDataCarSw	= getAplDataCarSw();
	inAplCtrlSet	= getAplCtrlSet();

	//通常モード
	aplComData.speed	= speedStabilize( inAplDataPalse->speed );		// 車速安定化処理(表示が暴れないように)
//	aplComData.speed	= makeTestDataSpeed();		// debugSpeed;
	aplComData.rev		= inAplDataPalse->rev;

	aplComData.palseSpeed	= inAplCtrlSet->palseSpeed;
	aplComData.palseRev		= inAplCtrlSet->palseRev; 

	if( inAplDataCarSw->ig == APL_DATA_SW_ON ){
		aplComData.ig	= APL_COM_SW_ON;
	}else{
		aplComData.ig	= APL_COM_SW_OFF;
	}
	if( inAplDataCarSw->acc == APL_DATA_SW_ON ){
		aplComData.acc	= APL_COM_SW_ON;
	}else{
		aplComData.acc	= APL_COM_SW_OFF;
	}
	if( inAplDataCarSw->ill == APL_DATA_SW_ON ){
		aplComData.ill	= APL_COM_SW_ON;
	}else{
		aplComData.ill	= APL_COM_SW_OFF;
	}
	if( inAplDataCarSw->vtc == APL_DATA_SW_ON ){
		aplComData.vtc	= APL_COM_SW_ON;
	}else{
		aplComData.vtc	= APL_COM_SW_OFF;
	}
}
//********************************************************************************
// LNKが取得
//********************************************************************************
APL_COM_DATA *getAplComData( void )
{
	return( &aplComData );
}
//********************************************************************************
// 車速安定化処理
//********************************************************************************
static uint8_t speedStabilize( uint16_t speed )
{
	speedBuff[speedBuffCnt++] = speed;
	if( speedBuffCnt >= SPEED_BUF_NUM ){
		speedBuffCnt = 0;
	}

	// 平均
	uint16_t	speedSum = 0;
	for( uint8_t i=0 ; i<SPEED_BUF_NUM ; i++ ){
		speedSum += speedBuff[i];
	}

	uint16_t	speedAverage = speedSum / SPEED_BUF_NUM;
	uint8_t		retSpeed=0;
	if( speedAverage < ROUND5 ){								// 0-1ブロック用
		retSpeed	= 0;
	}else if( speedAverage >= ((uint16_t)SPEED_MAX)*DIGIT_01KMPH){			// 異常
		retSpeed	= SPEED_MAX;
	}else{
		if( speedAverage <= (aplComData.speed*DIGIT_01KMPH) ){	// ==のときは、切り捨てになるのでここに入る
			retSpeed	= ((speedAverage + ROUND5) /DIGIT_01KMPH) - 1;	// ヒスにより変動しない幅を広げたい。20.1km/h -> 22.4km/h = 21km/hとしたい
		}else{
			retSpeed	= ((speedAverage + ROUND5) /DIGIT_01KMPH);
		}
	}

	return( retSpeed );
}

//********************************************************************************
// 車速テストデータ
//********************************************************************************
static unsigned char makeTestDataSpeed( void )
{
	if( testCycSpeed < TEST_CYC_SPEED ){
		testCycSpeed++;
	}else{
		testCycSpeed = 0;
		if( testStateSpeed == TEST_STATE_UP ){
			if( testSpeed < (SPEED_MAX-CHG_VAL_SPEED)){
				//1msに1rpm増加
				testSpeed += CHG_VAL_SPEED;
			}else{
				//状態を減少方向へ
				testSpeed = SPEED_MAX;
				testStateSpeed	= TEST_STATE_DOWN;
			}
		}else if( testStateSpeed == TEST_STATE_DOWN ){
			if( testSpeed > (SPEED_MIN+CHG_VAL_SPEED)){
				//1msに1rpm増加
				testSpeed -= CHG_VAL_SPEED;
			}else{
				//状態を減少方向へ
				testSpeed = SPEED_MIN;
				testStateSpeed	= TEST_STATE_UP;
			}
		}
	}
	return( testSpeed );
}
//********************************************************************************
// 回転数テストデータ
//********************************************************************************
static unsigned short makeTestDataRev( void )
{
	if( testCycRev < TEST_CYC_REV ){
		testCycRev++;
	}else{
		testCycRev = 0;
		if( testStateRev == TEST_STATE_UP ){
			if( testRev < (REV_MAX-CHG_VAL_REV)){
				//1msに1rpm増加
				testRev += CHG_VAL_REV;
			}else{
				//状態を減少方向へ
				testRev = REV_MAX;
				testStateRev	= TEST_STATE_DOWN;
			}
		}else if( testStateRev == TEST_STATE_DOWN ){
			if( testRev > (REV_MIN+CHG_VAL_REV)){
				//1msに1rpm増加
				testRev -= CHG_VAL_REV;
			}else{
				//状態を減少方向へ
				testRev = REV_MIN;
				testStateRev	= TEST_STATE_UP;
			}
		}
	}
	return( testRev );
}
