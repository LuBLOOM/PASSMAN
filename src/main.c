#include "../hdr/pm_account.h"
#include "../hdr/pm_aes.h"

#define PROMPTSIZE   1000
#define HELPMD       "help.md"

char *input;
int quit;

int passman_init(void);
int passman_prompt(void);
int passman_free(void);

int passman_pack_command(void);

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
		if (!strncmp("quit", input, 4)) {
			quit = 1;
		} else if (!strncmp("help", input, 4)) {
			FILE *help_fp = fopen(HELPMD, "r");
			for (int ch = 0; (ch = fgetc(help_fp)) != EOF; ) {
				fprintf(stdout, "%c", ch);
			}
			fclose(help_fp);
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

