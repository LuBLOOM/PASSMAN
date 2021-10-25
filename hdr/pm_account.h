#ifndef PM_ACCOUNT_H
#define PM_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pm_account {
	char *url;
	char *name;
	char *pass;
	struct pm_account *next;
};

struct pm_list {
	struct pm_account *head;
	void (* enc)(void);
	void (* dec)(void);
};

struct pm_list *accounts;
FILE *filp;

void pm_list_init(void (*)(void), void (*)(void));
void pm_list_insert(struct pm_account *);
void pm_list_get(char *, char*);
void pm_list_show(void);
void pm_list_free(void);

struct pm_account *pm_account_get(char *);
struct pm_account *pm_account_init(char *, char *, char *);


#endif /* PM_ACCOUNT_H */
