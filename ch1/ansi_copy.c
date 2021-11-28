#include <stdio.h>

int main(void)
{
	FILE *fp = NULL;
	
	fp = fopen("data.txt", "r");
	char buf[BUFSIZ];

	fgets(buf, BUFSIZ, fp);

	FILE *fp2 = fopen("data_copy_ansi.txt", "w");
	fputs(buf, fp2);

	fclose(fp);
	fclose(fp2);

	return 0;
}
	
