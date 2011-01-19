#include<stdio.h>
#include<errno.h>
int main()
{
	int err = 0;
	printf("Hello, OS!!!\n");
	err = errno;
	if (err) 
		{
			fprintf(stderr,"Hello, OS!!!\n");
		}
	return err;
}