#include<stdlib.h>
int main()
{
	long long int counter = 0;
	long int* a;
	if ((a = malloc(100000000*sizeof(long int)))==NULL) {return 0;}
	/*if ((a = malloc(999999999*sizeof(long int)))==NULL) {return 0;}*/
	int i;
	int j;
	for	(i=0;i<10000;i++)
	{
		for (j=0;j<10000;j++)
		{
			counter+=a[j*10000+i];
		}
	}
	free(a);
	return 1;
}
