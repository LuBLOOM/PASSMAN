#ifndef PM_AES_H_
#define PM_AES_H_

#include <string.h>
#include <stdlib.h>

static unsigned char pm_aes_sb[256];
static unsigned char pm_aes_isb[256];
static unsigned char pm_aes_mult2[256];
static unsigned char pm_aes_mult3[256];
static unsigned char pm_aes_rcon[256];

static unsigned char pm_aes_state[16];

static unsigned char key[16];
static unsigned char ekey[176];

void pm_aes_kexpc(unsigned char *, unsigned char);
void pm_aes_kexp(unsigned char *, unsigned char *);
void pm_aes_addroundkey(unsigned char *);
void pm_aes_subbytes(void);
void pm_aes_isubbytes(void);
void pm_aes_shiftrows(void);
void pm_aes_ishiftrows(void);
void pm_aes_mixcolumns(void);
void pm_aes_imixcolumns(void);
unsigned char *pm_aes_encrypt(unsigned char *);
unsigned char *pm_aes_decrypt(unsigned char *);
int  pm_aes_init(void);

#endif /* PM_AES_H_ */
