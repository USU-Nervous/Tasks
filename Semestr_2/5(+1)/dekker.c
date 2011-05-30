#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define SIZE 100
#define FIRST_COUNTER 1
#define SECOND_COUNTER 0
#define COUNT 10000000

static volatile unsigned int flags[]={0,0};
static volatile unsigned int turn=0;

//static int inv_t(int);
void die(int exitCode, char * message)
{
	printf("%s", message);
	exit(exitCode);
}

static void enter(int num)
{
	flags[num]=1;
	asm volatile("mfence" : : : "memory");
	while (flags[1-num])
		{
			if (turn!=num) 
			{flags[num]=0;asm volatile("mfence" : : : "memory");
			while (turn!=num) {};
			flags[num]=1;
			}
		}
	return;
}

static void leave(int num)
{
	turn=1-num;
	asm volatile("mfence" : : : "memory");	
	flags[num]=0;
	return;
}

volatile long long array[SIZE];

static void *thread_counter(void *args)
{
	int id = 0;
	int num = (int)args;
	int x;
	
	for(x = 0; x < COUNT; ++x)
		{
			enter(num);
			array[id]++;
			leave(num);
		}
	pthread_exit(NULL);
}

//static int inv_t(int ax)
//{
//	return (ax==0) ? 1 : 0;
//}

int main(int argc, char *argv[])
{
	cpu_set_t cpu1, cpu2;
	CPU_ZERO(&cpu1);
	CPU_SET(0, &cpu1);
	CPU_ZERO(&cpu2);
	CPU_SET(1, &cpu2);

	int i;
	pthread_t thread1;
	pthread_t thread2;

	if (pthread_create(&thread1, NULL, thread_counter, (void*)FIRST_COUNTER) != 0)
		die(11, "failed to create thread 1");
	if (pthread_create(&thread2, NULL, thread_counter, (void*)SECOND_COUNTER) != 0)
		die(21, "failed to create thread 2");

	if (pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpu1) != 0)
		die(10, "failed to setaffinity thread 1");	
	if (pthread_setaffinity_np(thread2, sizeof(cpu_set_t), &cpu2) != 0)
		die(20, "failed to setaffinity thread 2");	
		
	if (pthread_join(thread1, NULL) != 0)
		die(12, "failed join thread 1");
	if (pthread_join(thread2, NULL) != 0)
		die(22, "failed join thread 2");
	printf("%d",array[0]);
	return 0;
}
