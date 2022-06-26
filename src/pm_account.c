#include "../hdr/pm_account.h"

int pm_init(void)
{
	FILE *fp = fopen(INFOFILE, "r");
	
	if (!fp) {
		fp = fopen(INFOFILE, "w");
		fclose(fp);
		return -1;
	}
	
	fclose(fp);
	return 0;
}

int pm_add(char *username, char *password)
{
	FILE *fp = fopen(INFOFILE, "a");
	int status = 0;
	
	if (username && password) {
		ssize_t u_len = strlen(username);
		ssize_t p_len = strlen(password);
		
		ssize_t buffer_len = u_len + p_len + 2;
		char *buffer = calloc(sizeof *buffer, buffer_len);
		
		sprintf(buffer, "%s:%s\n", username, password);
		fwrite(buffer, buffer_len, sizeof *buffer, fp);
		free(buffer);
	} else  {
		status = -1;
	}
	
	fclose(fp);
	return status;
}

int pm_delete(char *username)
{
	FILE *fp = fopen(INFOFILE, "r");
	FILE *fp_new = fopen(".tmp", "w");
	int status = 0;
	
	if (username) {
		ssize_t size = 1000;
		char *buffer = malloc(size);
		
		while (getline(&buffer, &size, fp) != -1) {
			int length = strlen(username);
			int curr_length = strlen(buffer);
			
			if (curr_length < length)
				length = curr_length;
			
			int i;
			for (i = 0; i < length && *(buffer+i)-*(username+i) == 0; i++)
				;
			
			if (i != length) {
				fwrite(buffer, size, sizeof *buffer, fp_new);
			}
		}
	}
	
	fclose(fp);
	fclose(fp_new);
	remove(INFOFILE);
	rename(".tmp", INFOFILE);
	return status;
}

int pm_search(char *username)
{
	FILE *fp = fopen(INFOFILE, "r");
	int status = -1;
	
	if (username) {
		ssize_t size = 1000;
		char *buffer = malloc(size);
		
		while (getline(&buffer, &size, fp) != -1) {
			int length = strlen(username);
			int curr_length = strlen(buffer);
			
			if (curr_length < length)
				length = curr_length;
			
			int i;
			for (i = 0; i < length && *(buffer+i)-*(username+i) == 0; i++)
				;
			
			if (i == length) {
				fprintf(stdout, "Password: %s", buffer + length+1);
				status = 0;
				break;
			}
		}
	}
	
	fclose(fp);
	return status;
}

int pm_showall(void)
{
	FILE *fp = fopen(INFOFILE, "r");
	int status = 0;
	ssize_t size = 1000;
	char *buffer = malloc(size);
	
	while (getline(&buffer, &size, fp) != -1) {
		fprintf(stdout, "%s", buffer);
	}
	
	fclose(fp);
	return status;
}
