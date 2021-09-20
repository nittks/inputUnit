
typedef struct{
	unsigned char	palseSpeed;
	unsigned char	palseRev;
}LNK_IN_PALSE_CNT;
	
	
extern void initLnkInPalseCnt( void );
extern void lnkInPalseCntMain( void );
extern void setLnkInPalseCnt( LNK_IN_PALSE_CNT *inP );
