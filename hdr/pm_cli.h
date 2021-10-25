#ifndef PM_CLI_H
#define PM_CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pm_cli_prompt(void);
static void pm_cli_parse(void);
static void pm_cli_response(void);
static void pm_cli_help(void);
static void pm_cli_parseline(char *, char**);

#endif /* PM_CLI_H */
