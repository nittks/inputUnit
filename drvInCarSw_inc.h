


#define		ON		0
#define		OFF		1

#define		PORT_IG		((PIND >>PD4) & 0x01)
#define		PORT_ACC	((PIND >>PD5) & 0x01)
#define		PORT_ILL	((PIND >>PD6) & 0x01)
#define		PORT_VTC	((PIND >>PD7) & 0x01)
#define		PORT_TEST	((PINB >>PB2) & 0x01)
