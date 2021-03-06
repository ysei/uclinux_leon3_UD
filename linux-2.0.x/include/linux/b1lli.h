/*
 * $Id: b1lli.h,v 1.1.1.1 1999/11/22 03:47:00 christ Exp $
 *
 * ISDN lowlevel-module for AVM B1-card.
 *
 * Copyright 1996 by Carsten Paeth (calle@calle.in-berlin.de)
 *
 * $Log: b1lli.h,v $
 * Revision 1.1.1.1  1999/11/22 03:47:00  christ
 * Importing new-wave v1.0.4
 *
 * Revision 1.1.2.10  1998/10/25 14:37:35  fritz
 * Backported from MIPS (Cobalt).
 *
 * Revision 1.1.2.9  1998/03/20 14:30:02  calle
 * added cardnr to detect if you try to add same T1 to different io address.
 * change number of nccis depending on number of channels.
 *
 * Revision 1.1.2.8  1998/03/04 17:32:33  calle
 * Changes for T1.
 *
 * Revision 1.1.2.7  1998/02/27 15:38:29  calle
 * T1 running with slow link.
 *
 * Revision 1.1.2.6  1998/02/24 17:57:36  calle
 * changes for T1.
 *
 * Revision 1.1.2.5  1998/01/27 16:11:50  calle
 * support for PCMCIA B1/M1/M2 ready.
 *
 * Revision 1.1.2.4  1998/01/26 14:51:56  calle
 * interface change for pcmcia cards.
 *
 * Revision 1.1.2.3  1998/01/23 16:46:45  calle
 * new functions for pcmcia cards.
 *
 * Revision 1.1.2.2  1997/11/26 16:57:26  calle
 * more changes for B1/M1/T1.
 *
 * Revision 1.1.2.1  1997/11/26 10:47:01  calle
 * prepared for M1 (Mobile) and T1 (PMX) cards.
 * prepared to set configuration after load to support other D-channel
 * protocols, point-to-point and leased lines.
 *
 * Revision 1.1  1997/03/04 21:27:32  calle
 * First version in isdn4linux
 *
 * Revision 2.2  1997/02/12 09:31:39  calle
 * new version
 *
 * Revision 1.1  1997/01/31 10:32:20  calle
 * Initial revision
 *
 */

#ifndef _B1LLI_H_
#define _B1LLI_H_
/*
 * struct for loading t4 file 
 */
typedef struct avmb1_t4file {
	int len;
	unsigned char *data;
} avmb1_t4file;

typedef struct avmb1_loaddef {
	int contr;
	avmb1_t4file t4file;
} avmb1_loaddef;

typedef struct avmb1_loadandconfigdef {
	int contr;
	avmb1_t4file t4file;
        avmb1_t4file t4config; 
} avmb1_loadandconfigdef;

typedef struct avmb1_resetdef {
	int contr;
} avmb1_resetdef;

typedef struct avmb1_getdef {
	int contr;
	int cardtype;
	int cardstate;
} avmb1_getdef;

/*
 * struct for adding new cards 
 */
typedef struct avmb1_carddef {
	int port;
	int irq;
} avmb1_carddef;

#define AVM_CARDTYPE_B1	0
#define AVM_CARDTYPE_T1	1
#define AVM_CARDTYPE_M1	2
#define AVM_CARDTYPE_M2	3

typedef struct avmb1_extcarddef {
	int port;
	int irq;
        int cardtype;
        int cardnr;  /* for HEMA/T1 */
} avmb1_extcarddef;

#define	AVMB1_LOAD		0	/* load image to card */
#define AVMB1_ADDCARD		1	/* add a new card */
#define AVMB1_RESETCARD		2	/* reset a card */
#define	AVMB1_LOAD_AND_CONFIG	3	/* load image and config to card */
#define	AVMB1_ADDCARD_WITH_TYPE	4	/* add a new card, with cardtype */
#define AVMB1_GET_CARDINFO	5	/* get cardtype */
#define AVMB1_REMOVECARD	6	/* remove a card (usefull for T1) */



/*
 * card states for startup
 */

#define CARD_FREE	0
#define CARD_DETECTED	1
#define CARD_LOADING	2
#define CARD_INITSTATE	4
#define CARD_RUNNING	5
#define CARD_ACTIVE	6

#ifdef __KERNEL__

#define	AVMB1_PORTLEN		0x1f

#define AVM_MAXVERSION		8

#define AVM_NAPPS		30
#define AVM_NNCCI_PER_CHANNEL	4

/*
 * Main driver data
 */

typedef struct avmb1_card {
	struct avmb1_card *next;
	int cnr;
	unsigned int port;
	unsigned irq;
	int cardtype;
	int cardnr; /* for T1-HEMA */
	volatile unsigned short cardstate;
	int interrupt;
	int blocked;
	int versionlen;
	char versionbuf[1024];
	char *version[AVM_MAXVERSION];
	char msgbuf[128];	/* capimsg msg part */
	char databuf[2048];	/* capimsg data part */
	capi_version cversion;
	char name[10];
} avmb1_card;

/*
 * Versions
 */

#define	VER_DRIVER	0
#define	VER_CARDTYPE	1
#define	VER_HWID	2
#define	VER_SERIAL	3
#define	VER_OPTION	4
#define	VER_PROTO	5
#define	VER_PROFILE	6
#define	VER_CAPI	7


/* b1lli.c */
int B1_detect(unsigned int base, int cardtype);
int T1_detectandinit(unsigned int base, unsigned irq, int cardnr);
void B1_reset(unsigned int base);
void T1_reset(unsigned int base);
int B1_load_t4file(unsigned int base, avmb1_t4file * t4file);
int B1_load_config(unsigned int base, avmb1_t4file * config);
int B1_loaded(unsigned int base);
void B1_setinterrupt(unsigned int base, unsigned irq, int cardtype);
unsigned char B1_disable_irq(unsigned int base);
void T1_disable_irq(unsigned int base);
int B1_valid_irq(unsigned irq, int cardtype);
int B1_valid_port(unsigned port, int cardtype);
void B1_handle_interrupt(avmb1_card * card);
void B1_send_init(unsigned int port,
	    unsigned int napps, unsigned int nncci, unsigned int cardnr);
void B1_send_register(unsigned int port,
		      __u16 appid, __u32 nmsg,
		      __u32 nb3conn, __u32 nb3blocks, __u32 b3bsize);
void B1_send_release(unsigned int port, __u16 appid);
void B1_send_message(unsigned int port, struct sk_buff *skb);

/* b1capi.c */
void avmb1_handle_new_ncci(avmb1_card * card,
			   __u16 appl, __u32 ncci, __u32 winsize);
void avmb1_handle_free_ncci(avmb1_card * card,
			    __u16 appl, __u32 ncci);
void avmb1_handle_capimsg(avmb1_card * card, __u16 appl, struct sk_buff *skb);
void avmb1_card_ready(avmb1_card * card);

/* standard calls, with check and allocation of resources */
int avmb1_addcard(int port, int irq, int cardtype);
int avmb1_probecard(int port, int irq, int cardtype);

int avmb1_resetcard(int cardnr);

/* calls for pcmcia driver */
int avmb1_detectcard(int port, int irq, int cardtype);
int avmb1_registercard(int port, int irq, int cardtype, int allocio);
int avmb1_unregistercard(int cnr, int freeio);

#endif				/* __KERNEL__ */

#endif				/* _B1LLI_H_ */
