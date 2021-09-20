
typedef struct{
	unsigned short	currentTime;
	unsigned char	cycleTime;
	void			(*func)(void);
}TASK_PARAMETER;
//CS0 �N���b�N�I��0
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

//WGM0 �g�`�������
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

//COM0A ��rA�o�͑I��
enum{
	COM0A_NORMAL	= 0,
	COM0A_COMP_TOGLE,
	COM0A_COMP_LOW,
	COM0A_COMP_HIGH
};

//���W�X�^�ݒ�
#define		REG_CS0		(CS0_DIV_64 & 0x07)	//3bit
#define		REG_WGM		(WGM_COMP_CTC & 0x07)		//3bit
#define		REG_COM0A	(COM0A_NORMAL & 0x03)	//2bit
#define		TIMER_TOP	125		//8us*125=1ms,1ms�����荞��
#define		REG_OCIE0A	1		//��r��v�����݋���
#define		REG_CAL		88		//�����N���b�N�Z��(�f�t�H���g88(0x58)
#define		PCINT		((1<<PCINT1) | (1<<PCINT0))


//���W�X�^�Z�b�g�p
#define		SET_TCCR0A	(( REG_COM0A << COM0A0 ) | ((REG_WGM & 0x03) << WGM00))	//WGM00,01�̂�
#define		SET_TCCR0B	(( (REG_WGM >> 2) << WGM02) | (REG_CS0 << CS00))
#define		SET_OCR0A	TIMER_TOP
#define		SET_TIMSK0	(( REG_OCIE0A << OCIE0A))
#define		SET_OSCCAL	REG_CAL
#define		SET_PCMSK	PCINT



enum{
	TASK_DRV_IN_MAIN,
	TASK_LINK_IN,
	TASK_APL,
	TASK_LINK_OUT,
	TASK_DRV_OUT,
	TASK_TIMER,
	TASK_POWER_LED,
	TASK_MAX
};
static void powerLed( void );

TASK_PARAMETER	taskParameter[TASK_MAX]	={
	//���ݎ���(�J�n���I�t�Z�b�g) , ���� , �֐���
	{	1,	10,	drvInMain	},
	{	2,	10,	lnkInMain	},
	{	3,	10,	aplMain		},
	{	4,	10,	lnkOutMain	},
	{	5,	10,	drvOutMain	},
	{	6,	1,	timerMain	},
	{	7,	250,powerLed	},
};




