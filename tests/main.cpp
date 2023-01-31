#  include <iostream>
#  include <algorithm>
#  include <vector>
#  include <cstdio>
#  include <cstdlib>
#  include <ctime>
#  include <sys/time.h>

const int N = 1e7+7;
const int MOD = 934934;

int main(int argc,char **argv) {
	std::vector<int> v(N, 0);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec*1000+tv.tv_usec/1000);
	for(int i=0; i < 1e7+7; i++) v[i] = rand() % MOD;
	clock_t c=clock();
	std::sort(v.begin(), v.end());
	fprintf(stderr, "std::sort took %.5f sec...\n", ((float) (clock()-c)/CLOCKS_PER_SEC));
	return 0;
}
