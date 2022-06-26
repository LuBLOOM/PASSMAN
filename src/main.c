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

void trim(char **);

int main(void)
{
	if (passman_init() != 0) {
		fprintf(stderr, "passman> Could not start, there was an error\n");
	}
	passman_prompt();
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
		trim(&input);
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
			
		} else if (!strncmp("search", input, 6)) {
			fprintf(stdout, "search'ing for the username\n");
		} else if (!strncmp("delete", input, 6)) {
			fprintf(stdout, "delete'ing the username and password\n");
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

void trim(char **s)
{
	char *ref = *s;
	int first = -1, last = -1;
	for (int i = 0, c = 0; (c = *(ref+i)) != '\0'; i++) {
		if (first < 0 && c != ' ') {
			first = i;
		}
		if (c != ' ') last = i;
	}
	last += 1;
	int i;
	for (i = first; i < last; i++) {
		*(ref+i-first) = *(ref+i);
	}
	*(ref+i-first) = '\0';
}
