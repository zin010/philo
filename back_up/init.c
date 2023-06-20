/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:19:42 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/16 19:16:08 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_n_check(t_table *t, char **argv)
{
	if (!table_init(t, argv))
	{
		check_n_cleanup(t, "table_init() error");
		return (false);
	}
	if (!fork_init(t))
	{
		check_n_cleanup(t, "fork_init() error");
		return (false);
	}
	if (!philo_init(t, t->philo))
	{
		check_n_cleanup(t, "philo_init() error");
		return (false);
	}
	return (true);
}

int	table_init(t_table *t, char **argv)
{
	int	i;

	gettimeofday(&t->start, NULL);
	i = 0;
	while (++i <= 5 && argv[i])
	{
		if (ft_atoi(argv[i]) <= 0)
			return (false);
	}
	t->n_of_p = ft_atoi(argv[1]);
	t->t_die = ft_atoi(argv[2]);
	t->t_eat = ft_atoi(argv[3]);
	t->t_sleep = ft_atoi(argv[4]);
	if (argv[5])
		t->m_eat = ft_atoi(argv[5]);
	t->flag = NOTHING;
	t->check.fork_init = 0;
	t->check.philo_init = 0;
	t->check.key_init = 0;
	if (pthread_mutex_init(&t->key, NULL))
		return (false);
	t->check.key_init++;
	t->check.key_state = UNLOCK;
	return (true);
}

int	fork_init(t_table *t)
{
	int	i;

	t->philo = (t_philo **)malloc(sizeof(t_philo *) * t->n_of_p);
	t->cnt = (int *)malloc(sizeof(int) * t->n_of_p);
	t->check.fork_state = (int *)malloc(sizeof(int) * t->n_of_p);
	t->fork = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * t->n_of_p);
	if (!t->cnt || !t->fork || !t->check.fork_state || !t->philo)
		return (false);
	i = 0;
	while (i < t->n_of_p)
	{
		t->cnt[i] = 0;
		t->fork[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!t->fork[i])
			return (false);
		if (pthread_mutex_init(t->fork[i], NULL))
			return (false);
		t->check.fork_init++;
		t->check.fork_state[i] = UNLOCK;
		i++;
	}
	return (true);
}

int	philo_init(t_table *t, t_philo **p)
{
	int	i;

	i = 0;
	while (i < t->n_of_p)
	{
		p[i] = (t_philo *)malloc(sizeof(t_philo));
		if (!p[i])
			return (false);
		p[i]->table = (void *)t;
		p[i]->ate_ms = 0;
		p[i]->slept_ms = 0;
		p[i]->num = i + 1;
		p[i]->l_fork = t->fork[i];
		p[i]->key = &t->key;
		i++;
	}
	while (--i > 0)
		p[i]->r_fork = t->fork[i - 1];
	p[0]->r_fork = t->fork[t->n_of_p - 1];
	if (!philo_create(t, p))
		return (false);
	return (true);
}

int	philo_create(t_table *t, t_philo **p)
{
	int	i;

	i = 0;
	while (i < t->n_of_p)
	{
		if (pthread_create(&p[i]->me, NULL, routine, (void *)p[i]))
			return (false);
		t->check.philo_init++;
		i++;
	}
	return (true);
}
