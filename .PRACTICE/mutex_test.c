#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_data
{
	int				*cnt;
	char			*th;
	pthread_mutex_t	*mutex;
}			t_data;

void	*count(void *arg);

int	main(void)
{
	pthread_t		thread_1;
	pthread_t		thread_2;
	t_data			data_1;
	t_data			data_2;
	void			**rtn;
//	void			**rtn2;
	pthread_mutex_t	mutex;
	int				cnt;

	pthread_mutex_init(&mutex, NULL);
	cnt = -1;

	data_1.cnt = &cnt;
	data_2.cnt = &cnt;
	data_1.th = "thread 1";
	data_2.th = "thread 2";
	data_1.mutex = &mutex;
	data_2.mutex = &mutex;

	pthread_create(&thread_1, NULL, count, (void *)&data_1);
	pthread_create(&thread_2, NULL, count, (void *)&data_2);

	pthread_join(thread_1, rtn);
	printf("\t\t\t\t!%s joined!\n", (char *)*rtn);
//	pthread_join(thread_2, rtn2);
//	bus error
	pthread_join(thread_2, rtn);
	printf("\t\t\t\t?%s joined?\n", (char *)*rtn);

	pthread_mutex_destroy(&mutex);
	return (0);
}

void	*count(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	pthread_mutex_lock(data->mutex);
	printf("mutex locked : %s\n", data->th);
	*(data->cnt) = 0;
	for (i = 0; i < 10; i++)
	{
		printf("  %s cnt:%d i:%d\n", data->th, *(data->cnt), i);
		*(data->cnt) = *(data->cnt) + 1;
		usleep(1);
	}
	pthread_mutex_unlock(data->mutex);
	printf("mutex unlocked : %s\n\n", data->th);
	return ((void *)data->th);
}
