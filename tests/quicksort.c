#  include <stdio.h>
#  include <stdlib.h>
#  include <time.h>
#  include <sys/time.h>

# define N ((int) 1e7+7)

void quicksort(int a[], int l, int r);
void insertionsort(int a[], int n);

int main()
{
	static int a[N];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec*1000+tv.tv_usec);
	for(int i=0;i < N; i++) a[i] = rand() % N; //, printf("%d ", a[i]); printf("\n");
	clock_t c = clock();
	quicksort(a, 0, N-1);
	c = clock() - c;
	//for(int i=0;i < N; i++) printf("%d ", a[i]); printf("\n");
	printf("[+] Sorting took %.5f sec...\n", (float) c / CLOCKS_PER_SEC);
	return 0;
}

void quicksort(int a[], int l, int r) 
{
	if(r-l < 15) return insertionsort(a+l, r-l+1);

	int p = a[r];
	int i,j=l;
	for(i=l;i < r;i++) {
		if(a[i] < p) {
			int n = a[i];
			a[i] = a[j];
			a[j++] = n;
		}
	}
	a[r] = a[j];
	a[j] = p;
	quicksort(a, l, j-1);
	quicksort(a, j+1, r);
}

void insertionsort(int a[], int n)
{
	for(int i=1;i < n; i++) {
		int j = i;
		while(j > 0 && a[j-1] > a[j]) {
			int n = a[j];
			a[j] = a[j-1];
			a[j-1] = n;
			j--;
		}
	}	
}

