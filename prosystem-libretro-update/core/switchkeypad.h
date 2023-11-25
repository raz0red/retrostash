#ifndef  __SWITCHKEY_H__
#define  __SWITCHKEY_H__

#define SWITCHKEY_STATE                 "/sys/devices/platform/gpio-keys-polled/keys"

enum  {
	ASPECT_NONE=0,
	ASPECT_16_9,
	ASPECT_4_3,
};

/*
 * refer to retroarch: aspectratio_lut[]
*/
enum aspect_ratio
{
   ASPECT_RATIO_4_3 = 0,
   ASPECT_RATIO_16_9,
   ASPECT_RATIO_16_10,
   ASPECT_RATIO_16_15,
   ASPECT_RATIO_21_9,
   ASPECT_RATIO_1_1,
   ASPECT_RATIO_2_1,
   ASPECT_RATIO_3_2,
   ASPECT_RATIO_3_4,
   ASPECT_RATIO_4_1,
   ASPECT_RATIO_4_4,
   ASPECT_RATIO_5_4,
   ASPECT_RATIO_6_5,
   ASPECT_RATIO_7_9,
   ASPECT_RATIO_8_3,
   ASPECT_RATIO_8_7,
   ASPECT_RATIO_19_12,
   ASPECT_RATIO_19_14,
   ASPECT_RATIO_30_17,
   ASPECT_RATIO_32_9,
   ASPECT_RATIO_CONFIG,
   ASPECT_RATIO_SQUARE,
   ASPECT_RATIO_CORE,
   ASPECT_RATIO_CUSTOM,

   ASPECT_RATIO_END
};


enum  {
	SW_KEY_SEL     =    1<<0,
	SW_KEY_RST     =    1<<1,
	SW_KEY_BW      =    1<<2,
	SW_KEY_COLOR   =    1<<3,
	
	SW_KEY_1P_A    =    1<<4,
	SW_KEY_1P_B    =    1<<5,
	SW_KEY_2P_A    =    1<<6,
	SW_KEY_2P_B    =    1<<7,

	SW_KEY_16_9    =    1<<8,
	SW_KEY_4_3     =    1<<9,
};


extern int switchpad_update();
extern int get_atari_card_state();
extern int switchpad_init();
extern void switchpad_deinit();

#endif
