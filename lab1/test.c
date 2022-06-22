#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    char buf[4096] = {0};
    char line[512] = {0};

    fgets(line, 512, stdin);    
    line[strlen(line)-1] = '\0';

	FILE* fp = fopen("/dev/var4", "r+");
	if(fp == NULL)
    {
		printf("open var4 failed!!!\n");
		return -1;
	}
	
    fwrite(line, strlen(line), 1, fp);
	fseek(fp,0,SEEK_SET);
    fread(buf, sizeof(buf), 1, fp);
    printf("%s\n", buf);
    fclose(fp);
	return 0;
}
