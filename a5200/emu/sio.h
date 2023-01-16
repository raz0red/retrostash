/* $Id: sio.h,v 1.12 2005/08/17 22:45:54 pfusik Exp $ */
#ifndef _SIO_H_
#define _SIO_H_

#include "config.h"

#include <stdio.h> /* FILENAME_MAX */

#include "atari.h"

#define MAX_DRIVES 8

typedef enum tagUnitStatus {
	Off,
	NoDisk,
	ReadOnly,
	ReadWrite
} UnitStatus;

extern UnitStatus drive_status[MAX_DRIVES];
extern char sio_filename[MAX_DRIVES][FILENAME_MAX];

#define SIO_LAST_READ 0
#define SIO_LAST_WRITE 1

#define SIO_NoFrame         (0x00)
#define SIO_CommandFrame    (0x01)
#define SIO_StatusRead      (0x02)
#define SIO_ReadFrame       (0x03)
#define SIO_WriteFrame      (0x04)
#define SIO_FinalStatus     (0x05)
#define SIO_FormatFrame     (0x06)

UBYTE SIO_ChkSum(const UBYTE *buffer, int length);
void SwitchCommandFrame(int onoff);
void SIO_PutByte(int byte);
void SIO_Initialise(void);
void SIO_Exit(void);

/* Some defines about the serial I/O timing. Currently fixed! */
#define XMTDONE_INTERVAL  15
#define SERIN_INTERVAL     8
#define SEROUT_INTERVAL    8
#define ACK_INTERVAL      36

#endif	/* _SIO_H_ */
