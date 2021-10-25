#include "../hdr/pm_cli.h"
#include "../hdr/pm_utils.h"
#include "../hdr/pm_account.h"

static char *buffer;
static char *copy;
static char **arg_buf;
static char running;

void pm_cli_prompt(void)
{
	int i;
	buffer = (char *)malloc(BUFSIZE * sizeof *buffer);
	copy = (char *)malloc(BUFSIZE * sizeof *copy);
	arg_buf = (char **)malloc(MAXARGCOUNT * sizeof *arg_buf);
	running = 1;
	
	for (i = 0; i < MAXARGCOUNT; ++i) {
		char *tmp = (char *)malloc(BUFSIZE * sizeof *tmp);
		*(arg_buf+i) = tmp;
	}

	pm_list_init(NULL, NULL);

	char *line = (char *)malloc(BUFSIZE * sizeof *line);
	char **line_args = (char **)malloc(3 * sizeof *line_args);
	
	for (i = 0; i < 3; ++i) {
		char *tmp = (char *)malloc(BUFSIZE * sizeof *tmp);
		*(line_args+i) = tmp;
	}
	
	size_t size = BUFSIZE;
	filp = fopen("info/.information.info", "r");

	int line_size;
	for (; (line_size = getline(&line, &size, filp)) > 1;) {
		pm_cli_parseline(line, line_args);
		
		char *url = *(line_args+0);
		char *name = *(line_args+1);
		char *pass = *(line_args+2);
		struct pm_account *tmp = pm_account_init(url, name, pass);
		pm_list_insert(tmp);
	}

	for (i = 0; i < 3; ++i) {
		free(*(line_args+i));
	}
	
	free(line_args);
	free(line);
	fclose(filp);

	while (running) {
		printf("passman> ");

		fgets(buffer, 1000, stdin);
  
		pm_cli_parse();

		pm_cli_response();
	}
  
	free(buffer);
	free(copy);
	for (i = 0; i < MAXARGCOUNT; ++i) {
		free(*(arg_buf+i));
	}
	free(arg_buf);
	pm_list_free();
}

static void pm_cli_parse(void)
{
	int i, ptr, arg_c;

	for (i = 0; i < MAXARGCOUNT; ++i)
		memset(*(arg_buf+i), 0, sizeof **arg_buf);
  
	for (i = 0, ptr = 0, arg_c = 0; arg_c < MAXARGCOUNT; ++i, ++ptr) {
		if (*(buffer+i) == ' ') {
			*(copy+ptr) = '\0';
			memcpy(*(arg_buf+arg_c), copy, ptr+1);
			ptr = -1;
			arg_c++;
		} else if (*(buffer+i) == '\n') {
			*(copy+ptr) = '\0';
			memcpy(*(arg_buf+arg_c), copy, ptr+1);
			break;
		} else {
			*(copy+ptr) = *(buffer+i); 
		}
	}
}

static void pm_cli_response(void)
{
	if (0 == strcmp("quit", *(arg_buf+0))) {
		running = 0;
	} else if (0 == strcmp("help", *(arg_buf+0))) {
		pm_cli_help();
	} else if (0 == strcmp("show", *(arg_buf+0))) {
		pm_list_show();
	} else if (0 == strcmp("insert", *(arg_buf+0))) {
		int bi = ((0 < strcmp(*(arg_buf+1), ""))
			 && (0 < strcmp(*(arg_buf+2), ""))
			 && (0 < strcmp(*(arg_buf+3), "")));
		if (bi) {
			char *url = *(arg_buf+1);
			char *name = *(arg_buf+2);
			char *pass = *(arg_buf+3);
			struct pm_account *acc = pm_account_init(url, name, pass);
			pm_list_insert(acc);
		} else {
			printf("usage: insert <url> <username> <password>\n");
		}
	} else if (0 == strcmp("get", *(arg_buf+0))) {
		int bg = ((0 < strcmp(*(arg_buf+1), ""))
			  && (0 < strcmp(*(arg_buf+2), "")));
		if (bg) {
			char *url = *(arg_buf+1);
			char *name = *(arg_buf+2);
			pm_list_get(url, name);
		} else {
			printf("usage: insert <url> <name>\n");
		}
	}
}

static void pm_cli_help(void)
{
	printf("%s\n",
	       "help - Display this info\n"
	       "quit - Quit from the program\n"
	       "show - show current tracked accounts\n"
	       "insert <url> <name> <pass> - add an account\n"
	       "get <url> <name> - get the password of that account\n"
	       );
}


static void pm_cli_parseline(char *line, char **line_args)
{
	int i, line_ptr, arg_c;
	char *copy = (char *)malloc(BUFSIZE * sizeof *copy);
	for (i = 0, line_ptr = 0, arg_c = 0; i < *(line + i) != '\0'; i++, line_ptr++) {
		if (*(line+i) == '-' && *(line+i+1) == '>') {
			*(copy+line_ptr) = '\0';
			memcpy(*(line_args+arg_c), copy, BUFSIZE);
			line_ptr = -1;
			i++;
			arg_c++;
		} else {
			*(copy+line_ptr) = *(line+i);
		}
	}
	memcpy(*(line_args+arg_c), copy, BUFSIZE);
	free(copy);
}

