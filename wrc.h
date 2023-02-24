#ifndef WRC_INCLUDE
#define WRC_INCLUDE

#define GAMEPAD_COUNT 4
#define INP_LEFT    1
#define INP_RIGHT   1 << 1
#define INP_UP      1 << 2
#define INP_DOWN    1 << 3
#define INP_START   1 << 4
#define INP_SELECT  1 << 5
#define INP_A       1 << 6
#define INP_B       1 << 7
#define INP_X       1 << 8
#define INP_Y       1 << 9
#define INP_LBUMP   1 << 10
#define INP_LTRIG   1 << 11
#define INP_LTHUMB  1 << 12
#define INP_RBUMP   1 << 13
#define INP_RTRIG   1 << 14
#define INP_RTHUMB  1 << 15

#define OPT1    1
#define OPT2    1 << 1
#define OPT3    1 << 2
#define OPT4    1 << 3
#define OPT5    1 << 4
#define OPT6    1 << 5
#define OPT7    1 << 6
#define OPT8    1 << 7
#define OPT9    1 << 8
#define OPT10   1 << 9
#define OPT11   1 << 10
#define OPT12   1 << 11
#define OPT13   1 << 12
#define OPT14   1 << 13
#define OPT15   1 << 14
#define OPT16   1 << 15

extern unsigned int wrc_options;
extern unsigned int wrc_input_state[GAMEPAD_COUNT];
extern float wrc_input_state_analog[GAMEPAD_COUNT][4];
extern int wrc_mouse_x;
extern int wrc_mouse_y;
extern int wrc_buttons;

#define MOUSE_LEFT                  1
#define MOUSE_MIDDLE                1 << 1
#define MOUSE_RIGHT                 1 << 2
#define MOUSE_WHEEL_UP              1 << 3
#define MOUSE_WHEEL_DOWN            1 << 4
#define MOUSE_HORIZ_WHEEL_UP        1 << 5
#define MOUSE_HORIZ_WHEEL_DOWN      1 << 6

#endif