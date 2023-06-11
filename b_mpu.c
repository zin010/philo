/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:33:15 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/11 16:20:06 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

/**/#include <stdlib.h>/**/

typedef struct s_philo
{
	int				num;
	pthread_t		me;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	void			*table;
	long			last;
}					t_philo;

typedef struct s_table
{
	pthread_mutex_t	fork[4];
	t_philo			philo[4];
	int				times[4];
	struct timeval	start;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			m_eat;
	int				death;
}					t_table;

void	fork_init(t_table *table);
void	philo_init(t_table *t, t_philo *p);
void	fork_destroy(t_table *table);
void	take_forks(t_philo *p, t_table *t);
void	eat(t_philo *p, t_table *t);
void	go_to_sleep(t_philo *p, t_table *t);
void	thinking(t_philo *p, t_table *t);

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5)
	{
		printf("arguments error\n");
		return (1);
	}
	table.t_die = atoi(argv[1]);
	table.t_eat = atoi(argv[2]);
	table.t_sleep = atoi(argv[3]);
	table.m_eat = atoi(argv[4]);
	table.death = 0;
	gettimeofday(&table.start, NULL);
	fork_init(&table);
	philo_init(&table, &(table.philo));

	//monitering

	pthread_join(table.philo[0].me, NULL);
	pthread_join(table.philo[1].me, NULL);
	pthread_join(table.philo[2].me, NULL);
	pthread_join(table.philo[3].me, NULL);

	fork_destroy(&table);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*p;
	t_table	*t;

	p = (t_philo *)arg;
	t = (t_table *)p->table;
	while (1)
	{
		if (t->times[0] >= t->m_eat && t->times[1] >= t->m_eat
			&& t->times[2] >= t->m_eat && t->times[3] >= t->m_eat)
			break ;
		if (p->num % 2)
		{
			pthread_mutex_lock(p->r_fork);
			pthread_mutex_lock(p->l_fork);
		}
		else
		{
			pthread_mutex_lock(p->l_fork);
			pthread_mutex_lock(p->r_fork);
		}
		take_forks(p, t);
		eat(p, t);
		usleep(1000 * t->t_eat);
		pthread_mutex_unlock(p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		t->times[p->num - 1]++;
		if (t->times[0] >= t->m_eat && t->times[1] >= t->m_eat
			&& t->times[2] >= t->m_eat && t->times[3] >= t->m_eat)
			break ;
		go_to_sleep(p, t);
		usleep(1000 * t->t_sleep);
		thinking(p, t);
	}
	return (NULL);
}

int	are_they_all_alive(t_table *t)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (i < 4)
	{
		if (t->philo[i].live)
			cnt++;
		i++;
	}
	return (cnt == 4 - 1);
}

void	get_now_ms(t_table *t, long *now_ms)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	*now_ms = (now.tv_sec - t->start.tv_sec) * 1000 + now.tv_usec / 1000;
}

void	take_forks(t_philo *p, t_table *t)
{
	long	now;

	get_now_ms(t, &now);
	printf("%ld philo %d has taken a fork\n", now, p->num);
}

void	eat(t_philo *p, t_table *t)
{
	long	now;

	get_now_ms(t, &now);
	printf("%ld philo %d is eating %d\n", now, p->num, t->times[p->num - 1]);
}

void	go_to_sleep(t_philo *p, t_table *t)
{
	long	now;

	get_now_ms(t, &now);
	printf("%ld philo %d is sleep\n", now, p->num);
}

void	thinking(t_philo *p, t_table *t)
{
	long	now;

	get_now_ms(t, &now);
	printf("%ld philo %d is thinking\n", now, p->num);
}

void	fork_init(t_table *table)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		table->times[i] = 0;
		pthread_mutex_init(&(table->fork[i]), NULL);
		i++;
	}
}

void	philo_init(t_table *t, t_philo *p)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		p[i].num = i + 1;
		p[i].l_fork = &(t->fork[i]);
		p[i].table = (void *)t;
		p[i].last = t->start.tv_usec / 1000;
		i++;
	}
	i = 1;
	p[0].r_fork = &(t->fork[3]);
	while (i < 4)
	{
		p[i].r_fork = &(t->fork[i - 1]);
		i++;
	}
	i = 0;
	while (i < 4)
	{
		pthread_create(&(p[i].me), NULL, routine, (void *)&(p[i]));
		i++;
	}
}

void	fork_destroy(t_table *table)
{
	int	i;

	i = 0;
	while (i < 4)
		pthread_mutex_destroy(&(table->fork[i++]));
}
