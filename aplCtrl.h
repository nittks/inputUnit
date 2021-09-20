
//���
typedef enum{
	APL_CTRL_STATE_BOOT,		//�N������
	APL_CTRL_STATE_NOMARL		//�ʏ�
}APL_CTRL_STATE;

typedef struct {
	APL_CTRL_STATE		state;
}APL_CTRL;

typedef struct {
	unsigned char	palseSpeed;			//�p���X�d�l�ԑ�   
	unsigned char	palseRev;			//�p���X�d�l��]�� 
}APL_CTRL_SET;



extern void initAplCtrl( void );
extern void aplCtrlMain( void );
extern APL_CTRL *getAplCtrl( void );
extern APL_CTRL_SET	*getAplCtrlSet( void );
