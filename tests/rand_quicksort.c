#  include <stdio.h>
#  include <stdlib.h>
#  include <time.h>
#  include <sys/time.h>


# define N  ((int)17)

void rand_quicksort(int a[],int l,int r);
void print_arr(int a[],int n);

int main() {
	static int a[N];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec*1000+tv.tv_usec/1000);
	
	for(int i=0;i < N;i ++) a[i] = rand() % N;
	
	clock_t c = clock();
	rand_quicksort(a, 0, sizeof a/sizeof a[0] - 1);
	c = clock() -c;
	printf("rand_quicksort took %.5f sec...\n", (float) c/CLOCKS_PER_SEC);
	
	return 0;
}

void rand_quicksort(int a[],int l,int r)
{
	if(l>=r) return ;
	int rp = rand() % (r-l) + l;
	int p = a[rp];
	a[rp] = a[r];
	a[r] = p;

	int i,j=l;
	for(i=l;i < r;i++)  {
		if(a[i] < p) {
			int n=a[i];
			a[i] = a[j];
			a[j++] = n;
		}
	}

	a[r] = a[j];
	a[j] = p;

	rand_quicksort(a, j+1, r);
	rand_quicksort(a, l, j-1);
}

void print_arr(int a[],int n)
{
	for(int i=0;i < n;i++) {
		printf("%d ", a[i]);
	}
}
