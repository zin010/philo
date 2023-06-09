#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

void	*thread_routine(void *data);

int	main(void)
{
	pthread_t		p_thread[2];
	void			*rtn_th1;
	struct timeval	time;

	if (0 > pthread_create(&p_thread[0], NULL, thread_routine, (void *)"th_0"))
		perror("thread 0 cread fail");
	if (0 > pthread_create(&p_thread[1], NULL, thread_routine, (void *)"th_1"))
		perror("thread 1 cread fail");

	thread_routine((void *)"th_m");

	pthread_detach(p_thread[0]);
	pthread_join(p_thread[1], (void **) &rtn_th1);
	gettimeofday(&time, NULL);
	printf("\n%d  pthread 1 join : %s\n", time.tv_usec, (char *)rtn_th1);
	printf("p_thread[0] : %ld, ", (long)p_thread[0]);
	printf("p_thread[1] : %ld\n", (long)p_thread[1]);
	return (0);
}

void	*thread_routine(void *data)
{
	pid_t			pid;
	pthread_t		tid;
	struct timeval	time;

	pid = getpid();
	tid = pthread_self();
	for (int i = 0; i < 3; i++)
	{
		gettimeofday(&time, NULL);
		printf("%d %d  [%s] ", time.tv_usec, i, (char *)data);
		printf("pid : %d, tid %ld\n", pid, (long)tid);
		sleep(1);
	}
	return (NULL);
}
