#include "../hdr/pm_account.h"
#include "../hdr/pm_utils.h"

void pm_list_init(void (* enc)(void), void (* dec)(void))
{
	accounts = (struct pm_list *)malloc(sizeof *accounts);
	accounts->head = NULL;
	accounts->enc = enc;
	accounts->dec = dec;
}

void pm_list_insert(struct pm_account *acc)
{
	if (NULL == accounts->head) {
		accounts->head = acc;
	} else {
		struct pm_account *tmp = accounts->head;
		while (NULL != tmp->next)
			tmp = tmp->next;
		tmp->next = acc;
	}
	printf("%s->%s->%s\n", acc->url, acc->name, acc->pass);
}

void pm_list_get(char *url, char *name)
{
	struct pm_account *tmp = accounts->head;
	while (NULL != tmp) {
		if (0 == strcmp(tmp->name, name)
		    && 0 == strcmp(tmp->url, url)) {
			printf("account password: %s\n", tmp->pass);
			return;
		}
		tmp = tmp->next;
	}
	printf("could not find an account\n"
	       "use 'show' to display currently archived accounts\n");
}

void pm_list_show(void)
{
	struct pm_account *tmp = accounts->head;
	while (NULL != tmp) {
		printf("=== ACCOUNT START ===\n");
		printf("URL: %s\n", tmp->url);
		printf("NAME: %s\n", tmp->name);
		printf("=== ACCOUNT END   ===\n\n");
		tmp = tmp->next;
	}
}

void pm_list_free(void)
{
	struct pm_account *tmp = accounts->head;
	while (NULL != tmp) {
		struct pm_account *curr = tmp;
		tmp = tmp->next;
		free(curr->name);
		free(curr->url);
		free(curr->pass);
		free(curr);
	}
	accounts->enc = NULL;
	accounts->dec = NULL;
	free(accounts);
}

struct pm_account *pm_account_init(char *url, char *name, char *pass)
{
	struct pm_account *tmp =  (struct pm_account *)malloc(sizeof *tmp);
        tmp->url = (char *)malloc(BUFSIZE * sizeof *tmp->url);
	tmp->name = (char *)malloc(BUFSIZE * sizeof *tmp->name);
	tmp->pass = (char *)malloc(BUFSIZE * sizeof *tmp->pass);
	memcpy(tmp->url, url, 1000);
	memcpy(tmp->name, name, 1000);
	memcpy(tmp->pass, pass, 1000);
	return tmp;
}
