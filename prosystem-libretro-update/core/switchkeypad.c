#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "switchkeypad.h"

//#define  USE_SERIAL_CHECK_CARD


enum  drv_key_bit{
	BIT_SEL_KEY    =    1<<0,
	BIT_REST_KEY   =    1<<1,
	BIT_BW_COLOR   =    1<<2,
	BIT_1P_AB      =    1<<3,
	BIT_2P_AB      =    1<<4,
	BIT_1609_403   =    1<<5,

	BIT_CARD_DET   =    1<<6,
};

static int          threadcheck_run;
static int          atari_card_det;

int get_atari_card_state()
{
	return atari_card_det;
}

static int get_switchpad_state()
{
    FILE *fp = NULL;
    int ret;

    fp = fopen(SWITCHKEY_STATE, "r");
    if (!fp) {
        //printf("can't open %s\n", SPK_STATE);
        return -1;
    }
    if (fscanf(fp, "%d", &ret)) {
        fclose(fp);
        fp = NULL;
        return ret;
    }
    fclose(fp);
    fp = NULL;
    return -1;
}

int switchpad_update()
{
	int           curkey,report;

	if(0 == atari_card_det)
		return 0;
	
	curkey = get_switchpad_state();
	
	report = 0;
	
	if(BIT_SEL_KEY & curkey)
		report |= SW_KEY_SEL;
	
	if(BIT_REST_KEY & curkey)
		report |= SW_KEY_RST;
	
	if(BIT_BW_COLOR & curkey)
		report |= SW_KEY_BW;
	else
		report |= SW_KEY_COLOR;
	
	if(BIT_1P_AB & curkey)
		report |= SW_KEY_1P_B;
	else
		report |= SW_KEY_1P_A;
	
	if(BIT_2P_AB & curkey)
		report |= SW_KEY_2P_B;
	else
		report |= SW_KEY_2P_A;

	if(BIT_1609_403 & curkey) 
		report |= SW_KEY_16_9;
	else
		report |= SW_KEY_4_3;
	
	if(BIT_CARD_DET & curkey) 
		atari_card_det = 1;
	else
		atari_card_det = 0;

	//printf("--%s: report = %#x \n",__func__,curkey);
    return report;
}

//------------------------------------------

int switchpad_init()
{
	atari_card_det = 1;
	return 0;
}

void switchpad_deinit()
{
}


