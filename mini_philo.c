/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:33:15 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/09 17:30:44 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "philo.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_philo
{
	int				num;
	pthread_t		me;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	void			*table;
}					t_philo;

typedef struct s_table
{
	pthread_mutex_t	fork[4];
	t_philo			philo[4];
	int				times[4];
	int				cnt;
}					t_table;

void	fork_init(t_table *table);
void	philo_init(t_table *t, t_philo *p);
void	fork_destroy(t_table *table);

int	main(void)
{
	t_table	table;

	fork_init(&table);
	philo_init(&table, &(table.philo));

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
	if (t->times[0] >= 3 && t->times[1] >= 3
		&& t->times[2] >= 3 && t->times[3] >= 3)
		t->cnt = 1;
	while (!t->cnt)
	{
		pthread_mutex_lock(p->r_fork);
		printf("philo %d has taken a fork\n", p->num);
		printf("philo %d is eating\n", p->num);
		printf("philo %d is sleeping\n", p->num);
		printf("philo %d is thinking %d\n", p->num, t->times[p->num - 1]);
		t->times[p->num - 1]++;
	if (t->times[0] >= 3 && t->times[1] >= 3
		&& t->times[2] >= 3 && t->times[3] >= 3)
		t->cnt = 1;
		pthread_mutex_unlock(p->r_fork);
		usleep(100);
	}
	return (NULL);
}

void	fork_init(t_table *table)
{
	int	i;

	i = 0;
	table->cnt = 0;
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
