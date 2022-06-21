#ifndef PM_ACCOUNT_H_
#define PM_ACCOUNT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

#define INFOFILE ".accounts"
#define PAGEWIDTH 5

int pm_init(void);
int pm_add(char *username, char *password);
int pm_delete(char *username);
int pm_search(char *username);
int pm_showall(void);

#endif /* PM_ACCOUNT_H_ */
