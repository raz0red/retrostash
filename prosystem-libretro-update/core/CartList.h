#ifndef CARTLIST_H
#define CARTLIST_H

#include <ProSystem.h>

typedef struct cart
{
   char digest[256];
   uint8_t cart_type0;
   uint8_t cart_type1;
   uint8_t controller0;
   uint8_t controller1;
   uint8_t tv_type;
   uint8_t save_device;
   uint8_t xm;
} cart_t;

extern const struct cart cart_list[];
extern uint32_t get_cart_list_length();
#endif
