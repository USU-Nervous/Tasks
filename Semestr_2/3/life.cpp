#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdlib.h>

void quit(int exitCode, char * message){
	printf("%s", message);
	exit(exitCode);
}


const int MAX = 100;

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER; 

volatile bool INTACT = true;
volatile int nCol = 10, nRow = 10;
volatile bool cells[MAX][MAX];

int calcLife(int x, int y){
	int n = 0;
	bool fX = x<(nCol-1);
	if(y>0){
		if(cells[x][y-1]) n++;
		if(x>0 && cells[x-1][y-1]) n++;
		if(fX && cells[x+1][y-1]) n++;
	}
	if(x>0 && cells[x-1][y]) n++;
	if(fX && cells[x+1][y]) n++;
	if(y<(nRow-1)){
		if(cells[x][y+1]) n++;
		if(x>0 && cells[x-1][y+1]) n++;
		if(fX && cells[x+1][y+1]) n++;
	}
	return n;
}
void generateCells(){
	for(int y = 0; y < nRow; y++){
		for(int x = 0; x < nCol; x++)
			{cells[y][x] = (rand()+rand()+rand()+rand())%2;}
	}
}

void draw(){
	printf("```````````````````````````\n");
	for(int y = 0; y < nRow; y++){
		for(int x = 0; x < nCol; x++)
			{printf("%c", cells[y][x] ? 'x' : ' ');}
		printf("\n");
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

static void *life_thread(void *args){
	srand(time(NULL));
	generateCells();
	while(INTACT)
		{
			bool ncells[MAX][MAX];
			for(int y = 0; y < nRow; y++)
			for(int x = 0; x < nCol; x++)
				{
				int n = calcLife(y,x);
				if(cells[y][x]) ncells[y][x] = n==2 || n==3;
				else ncells[y][x] = n==3;
				}
		
			pthread_mutex_lock(&print_lock);
			//for(int y = 0; y < nRow; y++)
			//	for(int x = 0; x < nCol; x++)
			//		{cells[y][x] = ncells[y][x];}
                              bool** __cells = cells;
                              cells = ncells;
                              ncells = __cells;
			pthread_mutex_unlock(&print_lock);
			sleep(1);
		}
}

static void *print_thread(void *args){
	char c;
	while(INTACT)
	{
		if (scanf("%c", &c)){
		if (c=='q') {INTACT=false;break;}
		pthread_mutex_lock(&print_lock);
		draw();
		pthread_mutex_unlock(&print_lock);
		}

	}
}


int main() {
	pthread_t thread1;
	pthread_t thread2;

	if (pthread_create(&thread1, NULL, life_thread, NULL) != 0)
		quit(11, (char*)"failed to create thread 1");
	if (pthread_create(&thread2, NULL, print_thread, NULL) != 0)
		quit(21, (char*)"failed to create thread 2");

	if (pthread_join(thread1, NULL) != 0)
		quit(12, (char*)"failed join thread 1");
	if (pthread_join(thread2, NULL) != 0)
		quit(22, (char*)"failed join thread 2");
	return 0;
}

