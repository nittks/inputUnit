


#define		ON		0
#define		OFF		1

#define		PORT_IG		((PORTD.IN >> 4) & 0x01)
#define		PORT_ACC	((PORTD.IN >> 5) & 0x01)
#define		PORT_ILL	((PORTD.IN >> 6) & 0x01)
#define		PORT_VTC	((PORTD.IN >> 7) & 0x01)
#define		PORT_TEST	((PORTB.IN >> 2) & 0x01)
