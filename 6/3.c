#define MAX 1000
int a[MAX][MAX];
int main(){
	int sum, i, j, k;
	for(k = 0; k < 100; k++)
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
			sum += a[j][i];
}
