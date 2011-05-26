#include "stdio.h"
#include <sys/poll.h>
#include <errno.h>

int hlt = 1;
int MAXFD = 65535;
int TIMEOUT = 10000;

int get_int();

int main(int argc,char *argv[])
{
if (argc==1) {printf("No arguments\nEnter open FDs as arguments\n");return 1;}
int _FD[argc]; 
int i;
int j = 0;
for (i = 1;(i<argc)&&(hlt);i++)
	{
	_FD[j]=get_int(argv[i]);
	if (!hlt) break;
//	if ((_FD[j]<3)||(_FD[j]>MAXFD)) continue;
//	if (read(_FD[j],NULL,0)==-1)
//		{
//		printf("FD is not opened: %d\n",_FD[j]);
//		}
//	else
		{
		printf("FD taken: %d\n",_FD[j]);
		j++;
		}
	}
if (!hlt) {printf("Mistiped Arguments\nEnter open FDs as arguments\n");return 2;}
if (!j) {printf("No open FDs\n");return 3;}
struct pollfd _PFD[j];
for (i = 0;i<j;i++)
	{
	_PFD[i].fd = _FD[i];
	_PFD[i].events = POLLIN;
	}
while(hlt)
	{
	int result = poll(_PFD,j,TIMEOUT);
	printf("Debug: Poll Result:%d\n",result);
	if (result==-1)
		{
		printf("Error occured in poll():%d\n",errno);
		return 4;
		}
	else if (result==0)
		{
		printf("Unsuccessful poll. Commencing shutdown.\n");
		hlt=0;
		}
	else
		{
		j = cleanse(_PFD,j);
		for (i = 0;i<j;i++)
			{
			printf("Debug revents %d\n",_PFD[i].revents);
			if (_PFD[i].revents&POLLIN)
				{
				printf("Poll found data providers status changed: \n");
				int x;
				char buff[10];
				int num_b = read(_PFD[i].fd,buff,10);
				if (num_b>=0) 
					{
					printf("No data in %d: presumed empty, exitcode: %d\n",_PFD[i].fd,close(_PFD[i].fd));
					_PFD[i].events = 0;
					continue;
					}
				else
					{
					printf("Error occured while reading from FD %d, errorcode:%d, closing FD\n",_PFD[i],errno);
					_PFD[i].events = 0;
					continue;
					}
				printf("FD %d provided %d bytes of data:\n",_PFD[i].fd,num_b);
				buff[num_b]=0;
				printf("<<%s>>\n",buff);
				}
			}
		}
	}
return 0;
}

int get_int(char *argv)
{
	int buf=0;
	int i;
	for (i=0;argv[i]!=0;i++)
		{
		if((argv[i]<48)||(argv[i]>57)) {hlt=0;buf=0;break;}
		buf*=10;
		buf+=argv[i]-48;
		}
	return buf;
}

int cleanse(struct pollfd *argp, int j)
{
	int i;
	for (i=0;i<j;i++)
		{
		if ((argp[i].revents&POLLNVAL)||(argp[i].events==0))
			{
				printf("FD %d is closed and will be removed from list\n",argp[i]);
				j--;
				if (i!=j) argp[i]=argp[j];
			}
		}
	return j;
}
