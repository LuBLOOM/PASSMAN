#include "../hdr/pm_account.h"
#include "../hdr/pm_aes.h"
#include "../hdr/help.h"

#define PROMPTSIZE   1000
#define HELPMD       "help.txt"

static char *input;
static int quit;

int passman_init(void);
int passman_prompt(void);
int passman_free(void);

void trim(char **, char);
char *strdelim(char *, const char );
char *strdelim_l(char *, const char , char **);

int main(void)
{
	if (passman_init() != 0) {
		fprintf(stderr, "passman> Could not start, there was an error\n");
	}
	//passman_prompt();
	pm_aes_init();
	passman_free();
	return 0;
}

int passman_init(void)
{
	int status = 0;
	input = malloc(PROMPTSIZE);	

	if (!input)
		status = -1;
	return status;
}

int passman_prompt(void)
{
	while (!quit) {
		fprintf(stdout, "passman> ");
		int i, c;
		for (i = 0; (c = fgetc(stdin)) != '\n' && i < PROMPTSIZE - 1; i++) {
			*(input+i) = c;
		}
		*(input+i) = '\0';
		trim(&input, ' ');
		if (!strncmp("quit", input, 4)) {
			quit = 1;
		} else if (!strncmp("help", input, 4)) {
			/*
			FILE *help_fp = fopen(HELPMD, "r");
			if (!help_fp) {
				fprintf(stderr, "%s was not found\n", HELPMD);
				return -1;
			}
			for (int ch = 0; (ch = fgetc(help_fp)) != EOF; ) {
				fprintf(stdout, "%c", ch);
			}
			fclose(help_fp);
			*/
			fprintf(stdout, "%s\n", help_msg);
		} else if (!strncmp("add", input, 3)) {
			char *username = calloc(500, sizeof *username);
			char *password = calloc(500, sizeof *password);
			char delim = ' ';
			
			char *s = strdelim(input, delim);
			
			s = strdelim(NULL, delim);
			memcpy(username, s, strlen(s));
			
			s = strdelim(NULL, delim);
			memcpy(password, s, strlen(s));

			pm_add(username, password);
			
			free(username);
			free(password);
		} else if (!strncmp("search", input, 6)) {
			char *username = calloc(500, sizeof *username);
			char delim = ' ';

			char *s = strdelim(input, delim);

			s = strdelim(NULL, delim);
			memcpy(username, s, strlen(s));

			pm_search(username);
			
			free(username);
		} else if (!strncmp("delete", input, 6)) {
			char *username = calloc(500, sizeof *username);
			char delim = ' ';

			char *s = strdelim(input, delim);

			s = strdelim(NULL, delim);
			memcpy(username, s, strlen(s));

			pm_delete(username);

			free(username);
		} else if (!strncmp("showall", input, 7)) {
			pm_showall();
		} else {
			fprintf(stdout, "Invalid command. Try passman> 'help'\n");
		}
	}
}

int passman_free(void)
{
	free(input);
	
	return 0;
}

void trim(char **s, const char delim)
{
	char *ref = *s;
	int first = -1, last = -1;

	for (int i = 0, c = 0; (c = *(ref+i)) != '\0'; i++) {
		if (first < 0 && c != delim) {
			first = i;
		}
		if (c != delim) last = i;
	}

	
	last += 1;
	
	int i;	
	for (i = first; i < last; i++) {
		*(ref+i-first) = *(ref+i);
	}

	
	*(ref+i-first) = '\0';
}

/* similar to strtok and strtok_r in 'string.h' but implemented slightly differently */
char *strdelim(char *s, const char delim)
{
	static char *prev;
	
	return strdelim_l(s, delim, &prev);
}

char *strdelim_l(char *s, const char delim, char **prev)
{
	int c;
	char *new_s;

	if (s == NULL && (s = *prev) == NULL) 
		return NULL;

	/* remove trailing whitespace */
	for (c = *s; c == delim; c = *s++)
		;

	/* string might only contains delims */
	if (c == 0) {
		*prev = NULL;
		return NULL;
	}
	new_s = s;

	/* loop to find the end of the current sub string */
	for (;;) {
		c = *s++;
		if (c == delim) {
			if (c == 0)
				s = NULL;
			else
				s[-1] = 0;
			*prev = s;
			return new_s;
		}
	}
}
