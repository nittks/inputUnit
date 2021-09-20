//CS0 クロック選択0
enum{
	CS0_STOP	= 0,
	CS0_DIV_NO,
	CS0_DIV_8,
	CS0_DIV_64,
	CS0_DIV_256,
	CS0_DIV_1024,
	CS0_DIV_TODOWN,
	CS0_DIV_T0UP
};

//WGM0 波形生成種別
enum{
	WGM_NORMAL	=0,
	WGM_8BIT_PHASE_BASE_PWM,
	WGM_COMP_CTC,
	WGM_8BIT_HIGHT_SPEED_PWM,
	WGM_RESERVE,
	WGM_PHASE_BASE_PWM,
	WGM_RESERVE1,
	WGM_HIGHT_SPEED_PWM
};

//COM0A 比較A出力選択
enum{
	COM0A_NORMAL	= 0,
	COM0A_COMP_TOGLE,
	COM0A_COMP_LOW,
	COM0A_COMP_HIGH
};
#define	TIMER_REG_LSB		1		//1cnt=10us
#define	TIMER_REG_MAX		0xFFFF	//60ms毎、オーバーフロー割り込みを入れる

//レジスタ設定
#define		REG_COM1A	(0b00)		//標準ポート動作(OC1A切断)
#define		REG_WGM		(0b0000)	//no.1 標準動作
#define		REG_CS		(0x2)		//clkI/O(前置分周なし)10us単位でのカウントが必要のため、1Mhz(1us)クロックを使用
#define		REG_OCR1A	(TIMER_REG_MAX)	//TOP値設定

//レジスタセット用
#define		SET_TCCR1A	(TCCR1A = ( REG_COM1A << COM1A0 ) | ((REG_WGM & 0x03) << WGM00))	//WGM00,01のみ
#define		SET_TCCR1B	(TCCR1B = (( REG_WGM & 0x03 ) << WGM12))
#define		SET_TCCR1C	()	//セット値無し
#define		SET_OCR1A	(OCR1A = REG_OCR1A)
#define		SET_TIMSK	(TIMSK = (1 << OCIE1A ))		//タイマ/カウンタ1比較A割り込み許可

#define		START_TIMER_1A		(TCCR1B |= (  REG_CS << CS10))	//タイマスタート
#define		STOP_TIMER_1A		(TCCR1B &= (~(REG_CS << CS10)))	//タイマストップ
#define		EN_INTER_OVERFLOW_1A	(TIMSK1 |= (1<<TOIE1))
#define		DI_INTER_OVERFLOW_1A	(TIMSK1 &= ~(1<<TOIE1))

#define		GET_TIMER_CNT		(TCNT1)
#define		SET_TIMER_CNT(data)	(TCNT1=data)

#define		SEC60		60
#define		KMPERH60	60
#define		V1KMPERH	1
#define		N2			637
#define		NS			((unsigned long)1000000)	//sec->ns
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
const static unsigned long TIMER_CNT_MAX[N1_MAX] = {
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_04 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_08 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_16 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_20 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_25 * N2))
};

typedef enum{
	TIMER_STATE_STOP,
	TIMER_STATE_START,
	TIMER_STATE_OVERFLOW,
	TIMER_STATE_NUM
}TIMER_STATE;

typedef struct{
	TIMER_STATE		state;		//タイマ状態
	unsigned long	cnt;		//タイマカウント値
	unsigned short	startCnt;	//タイマ動作開始時、マイコンタイマレジスタ値
}TIMER_INS;
