#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#ifndef U_SEC
# define U_SEC 1000
#endif

//cc usleep_n_gettime.c -D U_SEC=100

int	main(int argc, char **argv)
{
	struct timeval	start;
	struct timeval	prev;
	struct timeval	next;
	long			prev_ms;
	long			next_ms;
	long			diff;

	gettimeofday(&start, NULL);
printf("start s:%d ms:%d us:%d\n", start.tv_sec, start.tv_usec / 1000, start.tv_usec);
	for (int i = 0; i < 15; i++)
	{
		gettimeofday(&prev, NULL);
		for (; 1;)
		{
			gettimeofday(&next, NULL);
			prev_ms = (prev.tv_sec - start.tv_sec) * 1000 + prev.tv_usec / 1000;
			next_ms = (next.tv_sec - start.tv_sec) * 1000 + next.tv_usec / 1000;
			diff = next_ms - prev_ms;
			if (diff >= 100)
				break ;
			usleep(U_SEC);
		}
printf(" %d diff:%d = n_ms:%d - p_ms:%d\n", i, diff, next_ms, prev_ms);
	}
}
