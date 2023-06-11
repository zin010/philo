#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

/**/#include <stdlib.h>/**/

#define LOCK 1
#define UNLOCK 0

#define NOTHING 0
#define DEATH 1
#define EXTERNAL 2
#define INTERNAL 3

typedef struct s_checklist
{
	int	fork_init;
	int	philo_init;
	int	fork_state[4];
}		t_checklist;//

typedef struct s_philo
{
	pthread_t		me;
	void			*table;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	long			ate_ms;
	int				num;
}

typedef struct s_table
{
	t_philo			philo[4];
	pthread_mutex_t	fork[4];
	int				cnt[4];
	struct timeval	start;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			m_eat;
	int				flag;
	t_checklist		check;
}					t_table;

void	check_n_cleanup(t_table *t, char *s);

int		init_n_check(t_table *t, char **argv);
int		table_init(t_table *t, char **argv);
int		fork_init(t_table *t);
int		philo_init(t_table *t, t_philo *p);

void	set_flag_n_usleep(t_table *t, int n);
int		get_now_ms(t_table *t, long *now_ms);
void	*routine(void *arg);

int		eat(t_philo *p, t_table *t);
int		go_to_sleep(t_philo *p, t_table *t);
int		think(t_philo *p, t_table *t);

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	record_fork_lock(t_philo *p, int right_handed, char c);
void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	record_fork_unlock(t_philo *p, int right_handed, char c);

//

/**********//**********//**********//**********//**********/
void	check_n_cleanup(t_table *t, char *s)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (t->check.fork_state[i])
			pthread_mutex_unlock(&(t->fork[i]));
		i++;
	}
	if (t->check.philo_init)
	{
		while (i < t->check.philo_init)
			pthread_join(table.philo[i++].me, NULL);
		i = 0;
	}
	if (t->check.fork_init)
	{
		while (i < t->check.fork_init)
			pthread_mutex_destroy(&(t->fork[i++]));
		i = 0;
	}
	if (s)
		printf("%s\n");
}
/**********//**********//**********//**********//**********/

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5)
	{
		check_n_cleanup(&table, "arguments error");
		return (EXIT_FAILURE);
	}
	if (!init_n_check(&table, argv))
		return (EXIT_FAILURE);
	//monitering//
	monitering(&table, &(table.philo));

	//joins
	//fork destroy
	return (EXIT_SUCCESS);
}


void	monitering(t_table *t, t_philo *p)
{
	while (true)
	{
		//
	}
}

/**********//**********//**********//**********//**********/
int	init_n_check(t_table *t, char **argv)
{
	if (!table_init(&table, argv))
	{
		check_n_cleanup("gettimeofday() error");
		return (false);
	}
	if (!fork_init(&table))
	{
		check_n_cleanup("pthread_mutex_init() error");
		return (false);
	}
	if (!philo_init(&table, &(table.philo)));
	{
		check_n_cleanup("pthread_create() error");
		return (false);
	}
	return (true);
}

int	table_init(t_table *t, char **argv)
{
	if (gettimeofday(&t->start, NULL))
		return (false);
	t->t_die = atoi(argv[1]);
	t->t_eat = atoi(argv[2]);
	t->t_sleep = atoi(argv[3]);
	t->m_eat = atoi(argv[4]);
	t->flag = NOTHING;
	t->check.fork_init = 0;
	t->check.philo_init = 0;
	return (true);
}

int	fork_init(t_table *t)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		t->cnt[i] = 0;
		if (pthread_mutex_init(&t->fork[i], NULL))
			return (false);
		t->check.fork_init++;
		t->check.fork_state[i] = UNLOCK;
		i++;
	}
	return (true);
}

int	philo_init(t_table *t, t_philo *p)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		p[i].table = (void *)t;
		p[i].l_fork = &(t_fork[i]);
		p[i].ate_ms = t->start.tv_usec / 1000;
		p[i].num = 1 + i++;
	}
	while (--i > 0)
		p[i].r_fork = &(t->fork[i - 1]);
	p[0].r_fork = &(t->fork[3]);
	while (i < 4)
	{
		if (pthread_create(&(p[i].me), NULL, routine, (void *)&(p[i])))
		{
			t->flag = EXTERNAL;
			return (false);
		}
		t->check.philo_init++;
		i++;
	}
	return (true);
}
/**********//**********//**********//**********//**********/

void	set_flag_n_usleep(t_table *t, int n)
{
	t->flag = n;
	usleep(1000);
}

int	get_now_ms(t_table *t, long *now_ms)
{
	struct timeval	now;

	if (gettimeofday(&now, NULL))
		return (false);
	*now_ms = (now.tv_sec - t->start.tv_sec) * 1000 + now.tv_usec / 1000;
	return (true);
}

//루틴은 !t->flag일 때만 실행되어야 함
//  t->flag일 때 : philo n의 죽음 or thread 생성 오류 or must eat 달성
//  죽음과 생성 오류, must eat 달성여부는 main thread에서 판단해줄 것
//  루틴은 주어진 역할만 성실히 수행하면 됨
//    근데 thread 내 오류 발생은 얘가 main한테 보고해줘야할 것 같음.
void	*routine(void *arg)
{
	t_philo	*p;
	t_table	*t;

	p = (t_philo *)arg;
	t = (t_table *)p->table;
	while (!t->flag)
	{
		if (p->num % 2)
			fork_lock_n_check(p->r_fork, p->l_fork, p);
		else
			fork_lock_n_check(p->l_fork, p->r_fork, p);
		if (!eat(p, t))
			break ;
		if (t->flag)
			break ;
		if (!go_to_sleep(p, t))
			break ;
		if (!think(p, t))
			break ;
	}
	return (NULL);
}

/**********//**********//**********//**********//**********/

int	eat(t_philo *p, t_table *t)
{
	if (t->flag)
		return (false);
	if (!get_now_ms(t, &(p->ate_ms)))
		set_flag_n_usleep(t, INTERNAL);
	if (t->flag)
		return (false);
	printf("%ld philo %d is eating\n", p->ate_ms, p->num);
	usleep(1000 * t->t_eat);
	if (p->num % 2)
		fork_unlock_n_check(p->r_fork, p->l_fork, p);
	else
		fork_unlock_n_check(p->l_fork, p->r_fork, p);
	t->cnt[p->num - 1]++;
}

int	go_to_sleep(t_philo *p, t_table *t)
{
	long	now;

	if (t->flag)
		return (false);
	if (!get_now_ms(t, &now))
		set_flag_n_usleep(t, INTERNAL);
	if (t->flag)
		return (false);
	printf("%ld philo %d is sleeping\n", now, p->num);
	usleep(1000 * t->t_sleep);
}

int	think(t_philo *p, t_table *t)
{
	long	now;

	if (t->flag)
		return (false);
	if (!get_now_ms(t, &now))
		set_flag_n_usleep(t, INTERNAL);
	if (t->flag)
		return (false);
	printf("%ld philo %d is thinking\n", now, p->num);
}

/**********//**********//**********//**********//**********/

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	int		right_handed;
	long	now;

	right_handed = p->num % 2;
	if (pthread_mutex_lock(f))
		set_flag_n_usleep(t, INTERNAL);
	record_fork_lock(p, right_handed, 'f');
	if (pthread_mutex_lock(s))
		set_flag_n_usleep(t, INTERNAL);
	record_fork_lock(p, right_handed, 's');
	if (!get_now_ms(t, &now))
		set_flag_n_usleep(t, INTERNAL);
	printf("%ld philo %d has taken a fork\n", now, p->num);
}

void	record_fork_lock(t_philo *p, int right_handed, char c)
{
	if (c == 'f')
	{
		if (right_handed) 
		{
			if (p->num == 1)
				t->check.fork_state[4 - 1] = LOCK;
			else
				t->check.fork_state[p->num - 2] = LOCK;
		}
		else
			t->check.fork_state[p->num - 1] = LOCK;
	}
	else
	{
		if (right_handed)
			t->check.fork_state[p->num - 1] = LOCK;
		else
		{
			if (p->num == 1)
				t->check.fork_state[4 - 1] = LOCK;
			else
				t->check.fork_state[p->num - 2] = LOCK;
		}
	}
}

void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	int		right_handed;

	right_handed = p->num % 2;
	if (pthread_mutex_unlock(f))
		set_flag_n_usleep(t, INTERNAL);
	record_fork_unlock(p, right_handed, 'f');
	if (pthread_mutex_unlock(s))
		set_flag_n_usleep(t, INTERNAL);
	record_fork_unlock(p, right_handed, 's');
}

void	record_fork_unlock(t_philo *p, int right_handed, char c)
{
	if (c == 'f')
	{
		if (right_handed) 
		{
			if (p->num == 1)
				t->check.fork_state[4 - 1] = UNLOCK;
			else
				t->check.fork_state[p->num - 2] = UNLOCK;
		}
		else
			t->check.fork_state[p->num - 1] = UNLOCK;
	}
	else
	{
		if (right_handed)
			t->check.fork_state[p->num - 1] = UNLOCK;
		else
		{
			if (p->num == 1)
				t->check.fork_state[4 - 1] = UNLOCK;
			else
				t->check.fork_state[p->num - 2] = UNLOCK;
		}
	}
}
/**********//**********//**********//**********//**********/
/**********//**********//**********//**********//**********/
