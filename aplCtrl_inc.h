#ifndef APL_CTRL_INC_H
#define APL_CTRL_INC_H

typedef enum{
	PALSE_SPEED,
	PALSE_REV,
	SETTING_ITEM_MAX
}SETTING_ITEM;

unsigned char eepDefault[SETTING_ITEM_MAX] = {
	4,		//パルス仕様車速
	4,		//パルス仕様回転数(気筒数
};

#endif
