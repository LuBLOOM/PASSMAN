#include <stdio.h>
#include <stdlib.h>

#define PM_FD "info/information.txt"
#define MAX_LINE 1024

#ifdef PM_DEBUG

#define PM_ERR "pmerr/passman.log"
#define PM_ERR_W "w"
#define PM_ERR_R "r"
#define PM_ERR_HEADER (100)
#define PM_ERR_MESSAGE (800)
#define PM_ERR_SIZE (PM_ERR_HEADER + PM_ERR_MESSAGE)
#define PM_ERR_COUNT (100)

#define PM_ERR_HEADER_PTR ("Pointer Exception: ")
#define PM_ERR_MESSAGE_PTR (__FILE__)


struct pm_err {
  FILE *filp;
  char *buffer;
  unsigned char pos;
};

static void pm_init_err(struct pm_err *);
static void pm_write_err(struct pm_err *, char *, char *);
static void pm_exit_err(struct pm_err *);

struct pm_err perr;

#endif /* PM_DEBUG */

#define MAXLINE 1000
#define MAXWORD 100

char enc_set[95] = {
  ' ', '!', '"', '#', '$', '%', '&', '(', ')', '*','+', ',',
  '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F',
  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
  'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '{', '|', '}', '~'
};

struct pm_account {
  char *url;
  char *pass;
  char *name;
  struct pm_account *next;
};

struct pm_list {
  struct pm_account *head;
  void (* encryptor)(char *);
  void (* decryptor)(char *);
};

static void pm_init_list(struct pm_list *, void (*)(char *), void (*)(char *));
static void pm_insert_list(struct pm_list *, struct pm_account *);
static void pm_encrypt_list(struct pm_list *);
static void pm_decrypt_list(struct pm_list *);
static void pm_show_list(struct pm_list *);
static void pm_free_list(struct pm_list *);

static struct pm_account *pm_get_account(char *);
static void pm_free_account(struct pm_account *);

static void cli_prompt(void);

static void enc_caesar(char *);
static void dec_caesar(char *);
static void enc_rot13(char *);
static void dec_rot13(char *);

static void pm_read(void);
static void pm_write(void);
static void pm_encrypt(void);
static void pm_decrypt(void);

int main(int argc, char **argv)
{
  #ifdef PM_DEBUG
  pm_init_err(&perr);
  #endif /* PM_DEBUG */

  struct pm_list accounts;
  pm_init_list(&accounts, enc_caesar, dec_caesar);
  
  FILE *fp = fopen(PM_FD, "r");
  int i, currsize;
  size_t linesize;
  char *buffer;

  for (i = 0, linesize = 0; (currsize = getline(&buffer, &linesize, fp)) > 1; ++i) {
    struct pm_account *account = pm_get_account(buffer);
    pm_insert_list(&accounts, account);
  }

  pm_show_list(&accounts);

  pm_free_list(&accounts);
  
  #ifdef PM_DEBUG
  pm_exit_err(&perr);
  #endif /* PM_DEBUG*/
  return 0;
}

#ifdef PM_DEBUG

static void pm_init_err(struct pm_err *perr)
{
  perr->buffer = (char *)malloc(PM_ERR_SIZE * sizeof *perr->buffer);
  perr->filp = fopen(PM_ERR, PM_ERR_W);
  perr->pos = 0;
}

static void pm_write_err(struct pm_err *perr, char *header, char *message)
{
  unsigned int i;
  perr->pos %= PM_ERR_COUNT;
  fseek(perr->filp, PM_ERR_SIZE * perr->pos, SEEK_SET);
  for (i = 0; i < PM_ERR_SIZE; ++i)
    *(perr->buffer+i) = 0;
  for (i = 0; i < PM_ERR_HEADER && (*(perr->buffer+i) = *(header+i)) != '\0'; ++i)
    ;
  for (i = PM_ERR_HEADER; i < PM_ERR_SIZE-1
	 && (*(perr->buffer+i) = *(message+(i-PM_ERR_HEADER))) != '\0'; ++i)
    ;
  *(perr->buffer+i) = '\n';
  fwrite(perr->buffer, sizeof *perr->buffer, PM_ERR_SIZE, perr->filp);
  perr->pos += 1;
}

static void pm_exit_err(struct pm_err *perr)
{
  free(perr->buffer);
  fclose(perr->filp);
  perr->buffer = NULL;
  perr->pos = 0;
}

#endif /* PM_DEBUG */

static void pm_init_list(struct pm_list *list, void (* enc)(char *), void (* dec)(char *))
{
  list->head = NULL;
  list->encryptor = enc;
  list->decryptor = dec;
}

static void pm_insert_list(struct pm_list *list, struct pm_account *acc)
{
  if (list->head == NULL)
    list->head = acc;
  else {
    struct pm_account *ptr = list->head;
    while (NULL != ptr->next)
      ptr = ptr->next;
    ptr->next = acc;
  }
}

static void pm_show_list(struct pm_list *list)
{
  struct pm_account *tmp = list->head;
  while (tmp != NULL) {
    printf("%s->%s->%s\n", tmp->url, tmp->name, tmp->pass);
    tmp = tmp->next;
  }
}

static void pm_free_list(struct pm_list *list)
{
  struct pm_account *tmp = list->head;
  struct pm_account *prev = NULL;
  while (tmp->next != NULL) {
    prev = tmp;
    tmp = tmp->next;
    pm_free_account(prev);
  }
  list->head = NULL;
}

static struct pm_account *pm_get_account(char *buffer)
{
  int i, os;
  char *name = (char *)malloc(MAXWORD * sizeof *name);
  char *pass = (char *)malloc(MAXWORD * sizeof *pass);
  char *url =  (char *)malloc(MAXWORD * sizeof *url);
  struct pm_account *account = (struct pm_account *)malloc(sizeof *account);
  for (i = 0, os = 0; (*(url+i) = *(buffer+i)) != '\n' && *(buffer+i) != '-' && *(buffer+i) != '>'; ++i)
    ;
  *(url+i) = '\0';
  os += i + 2;
  for (i = 0; (*(name+i) = *(buffer+i+os)) != '\n' && *(buffer+i+os) != '-' && *(buffer+i) != '>'; ++i)
    ;
  *(name+i) = '\0';
  os += i + 2;
  for (i = 0; (*(pass+i) = *(buffer+i+os)) != '\n'; ++i)
    ;
  *(pass+i) = '\0';
  account->name = name;
  account->pass = pass;
  account->url = url;
  return account;
}

static void cli_prompt(void)
{
  
}

static void pm_free_account(struct pm_account *acc)
{
  free(acc->url);
  free(acc->name);
  free(acc->pass);
  free(acc);
}

static void enc_caesar(char *buffer)
{
  int i;
  for (i = 0; *(buffer+i) != '\0'; ++i)
    ;
}

static void dec_caesar(char *buffer)
{
  
}

static void enc_rot13(char *buffer)
{
  
}

static void dec_rot13(char *buffer)
{

}
