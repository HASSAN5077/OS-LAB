#include<math.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
static void my_sig_handler(int signo)
{
	printf("Calling from handler\n");
}
int main(void)
{
	struct sigaction act;
	int count, count2;
	count = count2 = 0;
	double sum = 0;
	double x;
	for (;count < 100000; ++count)
		for(;count2 < 10000; ++count2)
		{
			x = (rand() + 0.5)/(RAND_MAX + 1.0);
			sum += sin(x);
		}
	act.sa_handler = my_sig_handler;
	act.sa_flags = 0;
	if ((sigaction(SIGINT, &act, NULL) == -1))
	{
		perror("Failed to set SIGINT handler");
		return 1;
	}
	sum = 0;
	printf("Signal Handler Installed\n");
	for (;count<10000000000; ++count)
		for(;count2<1000000000; ++count2)
		{
			x = (rand() + 0.5)/(RAND_MAX + 1.0);
			sum += sin(x);
		}
	return 0;
}