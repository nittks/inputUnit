

typedef enum{
	NO_SPEED,
	NO_REV,
	NO_MAX
}PORT_NO;

#define N1		4		//1回転当たりの車速センサパルス数
#define N2		637		//60km/h時の車速センサ回転速度rpm
#define C		2		//1回転当たりの点火回数
#define R_SP	2		//1点火当たりの回転数(4st=2,1st=1

#define INIT_PALSE_SPEED	4	//1回転当たりのパルス数
#define INIT_PALSE_REV		4	//気筒数

#define TIME_US	1000000		//usをsに換算し計算
#define ROUND_DIGIT	10		//小数点第1位で丸める。10倍して5を足して10で割る
#define ROUND5		5		//四捨五入。5を足して10で割る

const unsigned char SETTING_PALSE_SPEED[6]=	{0,4,8,16,20,25};		//車速パルス仕様
const unsigned char SETTING_PALSE_REV[9]={0,1,2,3,4,5,6,8,10};	//回転数パルス仕様(気筒数
