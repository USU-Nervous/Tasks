#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

const int EXIT_SIGNAL = 30;
int pipefd[2];

void sigHandler(int number, siginfo_t *info, void *something)
{
	int err = 0;
	int _pid = getpid();
	int _gid = getpgrp();
	if (write(pipefd[1], &number, sizeof(number))<0){err=1;};
	if (write(pipefd[1], &_pid, sizeof(number))<0){err=1;};
	if (write(pipefd[1], &_gid, sizeof(number))<0){err=1;};
	if (err) {stderr,write("pipefail");};
	if(number==EXIT_SIGNAL)
		 close(pipefd[1]);
}

int main()
{
	struct sigaction S;
	int number,_pid,_gid;

	if (pipe(pipefd) == -1) 
	{
		fprintf(stderr,"Failed to create a pipe");
        return 1;
    }

	S.sa_sigaction = sigHandler;
	S.sa_flags = SA_SIGINFO;
	if(sigfillset(&S.sa_mask))
	{
		fprintf(stderr,"Error:Signal mask is not set");
		return 2;
	}

	for(int i=1; i<32; ++i)
		if(i!=9 && i!=19 && sigaction(i, &S, NULL))
		{
			fprintf(stderr,"Error:Can't assing a handler");
			return 3;
		}

	do
	{
		int err = 0;
		if(read(pipefd[0], &number, sizeof(number)) > 0)
			{
			if (read(pipefd[0], &_pid, sizeof(number))<0){err=1;};
			if (read(pipefd[0], &_gid, sizeof(number))<0){err=1;};
			fprintf(stderr,"Signal %d arrived pid=%d, gid=%d\n", number, _pid, _gid);
			}
		else(err=1;)
		if (err) {write(stderr,"pipefail");};
	}
	while(number != EXIT_SIGNAL);
	return 0;
}