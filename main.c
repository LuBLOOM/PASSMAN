#include <stdio.h>
#include <stdlib.h>

#define PM_FD "information.txt"

#ifdef PM_DEBUG

#define PM_ERR "passman.log"
#define PM_ERR_W "w"
#define PM_ERR_R "r"
#define PM_ERR_HEADER (100)
#define PM_ERR_MESSAGE (800)
#define PM_ERR_SIZE (PM_ERR_HEADER + PM_ERR_MESSAGE)
#define PM_ERR_COUNT (10)

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

#endif /* PM_DEBUG */

struct pm_account {
  const char *url;
  const char *pass;
  const char *name;
  struct account *next;
};

struct pm_list {
  struct account *head;
};

static void pm_read(void);
static void pm_write(void);
static void pm_encrypt(void);
static void pm_decrypt(void);


int main(int argc, char **argv)
{
  #ifdef PM_DEBUG
  struct pm_err perr;
  pm_init_err(&perr);
  pm_write_err(&perr, PM_ERR_HEADER_PTR, PM_ERR_MESSAGE_PTR);
  pm_write_err(&perr, PM_ERR_HEADER_PTR, PM_ERR_MESSAGE_PTR);
  pm_exit_err(&perr);
  #endif /* PM_DEBUG*/
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
