#ifndef PM_AES_H_
#define PM_AES_H_

static char pm_aes_sb[256];
static char pm_aes_isb[256];
static char pm_aes_mult2[256];
static char pm_aes_mult3[256];
static char pm_aes_rcon[12];
static char pm_aes_state[16];

static char key[16];
static char ekey[176];

void pm_aes_kexpc(char *, char);
void pm_aes_kexp(char *, char *);
void pm_aes_addroundkey(char *);
void pm_aes_subbytes(void);
void pm_aes_shiftrows(void);
void pm_aes_mixcolumns(void);
char *pm_aes_encrypt(char *);
int  pm_aes_init(void);

#endif /* PM_AES_H_ */
