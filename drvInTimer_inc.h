#define	TIMER_REG_LSB		(0.1)	//1cnt=0.1us
#define	TIMER_REG_MAX		0xFFFF	//60ms毎、オーバーフロー割り込みを入れる

#define		START_TIMER		(TCB0.CTRLA |= TCB_ENABLE_bm)
#define		STOP_TIMER		(TCB0.CTRLA &= (~TCB_ENABLE_bm))
#define		EN_INTER_OVERFLOW	(TCB0.INTCTRL |= TCB_CAPT_bm)
#define		DI_INTER_OVERFLOW	(TCB0.INTCTRL &= (~TCB_CAPT_bm))

#define		GET_TIMER_CNT		(TCB0.CNT)
#define		SET_TIMER_CNT(data)	(TCB0.CNT=data)

#define		SEC60		60
#define		KMPERH60	60
#define		V1KMPERH	1
#define		N2			637
#define		US			((unsigned long)1000000)	//sec->us
enum{
	N1_04,
	N1_08,
	N1_16,
	N1_20,
	N1_25,
	N1_MAX
};

enum{
	N1_PALSE_04 = 4,
	N1_PALSE_08 = 8,
	N1_PALSE_16 = 16,
	N1_PALSE_20 = 20,
	N1_PALSE_25 = 25
};

//タイマカウント最大値(1km/h未満になる周期)。0㎞/h(オーバーフロー)判定用
volatile const static unsigned long TIMER_CNT_MAX_US[N1_MAX] = {
	(US*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_04 * N2)),
	(US*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_08 * N2)),
	(US*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_16 * N2)),
	(US*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_20 * N2)),
	(US*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_25 * N2))
};

typedef enum{
	TIMER_STATE_STOP,
	TIMER_STATE_START,
	TIMER_STATE_OVERFLOW,
	TIMER_STATE_NUM
}TIMER_STATE;

typedef struct{
	TIMER_STATE		state;		//タイマ状態
	unsigned long	cnt100ns;	//タイマカウント値 LSB 1=100ns
	unsigned short	startCnt;	//タイマ動作開始時、マイコンタイマレジスタ値
}TIMER_INS;
