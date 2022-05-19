//********************************************************************************
// 標準動作用定数
//********************************************************************************
#define		SPEED_MAX		((unsigned char)255)
#define		SPEED_MIN		((unsigned char)0)
#define		REV_MAX			((unsigned short)10000)
#define		REV_MIN			((unsigned short)0)

#define		SPEED_BUF_NUM	((uint8_t)(500/10))		// 500ms平均を取る。1周期10ms
#define		DIGIT_01KMPH	((uint8_t)10)			// 0.1km/hを1として扱うために乗算する
#define		ROUND5			((uint8_t)5)			// 四捨五入用
//********************************************************************************
// テストモード
//********************************************************************************
//テストデータ計算周期
#define		TEST_CYC_SPEED	((unsigned char)100-1)	//10x10ms=1000ms毎処理
#define		TEST_CYC_REV	((unsigned char)2-1)	//10x10ms=100ms毎処理
//計算毎の変化値
#define		CHG_VAL_SPEED	((unsigned char)2)
#define		CHG_VAL_REV		((unsigned char)200)
//ロータリークリック毎の変化値
#define		CHG_VAL_SPEED_MANUAL	((unsigned char)5)
#define		CHG_VAL_REV_MANUAL		((unsigned char)200)


typedef enum{
	TEST_STATE_UP,
	TEST_STATE_DOWN
}TEST_STATE;

//********************************************************************************
// 車体信号
//********************************************************************************
enum{
	APL_COM_SW_OFF,
	APL_COM_SW_ON
};
