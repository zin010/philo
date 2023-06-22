/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:45:35 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:53:25 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_n_check(t_table *t, char **argv)
{
	if (!table_init(t, argv))
	{
		check_n_cleanup(t, "table_init() error", 0);
		return (false);
	}
	if (!table_alloc(t))
	{
		check_n_cleanup(t, "table_alloc() error", 0);
		return (false);
	}
	if (!fork_init(t))
	{
		check_n_cleanup(t, "fork_init() error", 0);
		return (false);
	}
	if (!philo_init(t, t->philo))
	{
		check_n_cleanup(t, "philo_init() error", 0);
		return (false);
	}
	return (true);
}

int	table_init(t_table *t, char **argv)
{
	int				i;
	struct timeval	now;

	gettimeofday(&now, NULL);
	t->ref.s_sec = now.tv_sec;
	t->ref.s_usec = now.tv_usec;
	i = 0;
	while (++i <= 5 && argv[i])
	{
		if (ft_atoi(argv[i]) <= 0)
			return (false);
	}
	t->ref.n_of_p = ft_atoi(argv[1]);
	t->ref.t_die = ft_atoi(argv[2]);
	t->ref.t_eat = ft_atoi(argv[3]);
	t->ref.t_sleep = ft_atoi(argv[4]);
	if (argv[5])
		t->ref.m_eat = ft_atoi(argv[5]);
	t->flag = NOTHING;
	return (true);
}

int	table_alloc(t_table *t)
{
	int	n_of_p;

	n_of_p = t->ref.n_of_p;
	t->philo = (t_philo **)malloc(sizeof(t_philo *) * n_of_p);
	t->fork = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * n_of_p);
	t->m_ate = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * n_of_p);
	t->m_cnt = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * n_of_p);
	t->m_flag = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * n_of_p);
	t->check.fork_state = (int *)malloc(sizeof(int) * n_of_p);
	t->check.ate_state = (int *)malloc(sizeof(int) * n_of_p);
	t->check.cnt_state = (int *)malloc(sizeof(int) * n_of_p);
	t->check.flag_state = (int *)malloc(sizeof(int) * n_of_p);
	if (!t->philo || !t->fork || !t->m_ate || !t->m_cnt || !t->m_flag)
		return (false);
	if (!t->check.fork_state || !t->check.ate_state
		|| !t->check.cnt_state || !t->check.flag_state)
		return (false);
	memset((void *)t->check.fork_state, 0, sizeof(t->check.fork_state));
	memset((void *)t->check.ate_state, 0, sizeof(t->check.ate_state));
	memset((void *)t->check.cnt_state, 0, sizeof(t->check.cnt_state));
	memset((void *)t->check.flag_state, 0, sizeof(t->check.flag_state));
	return (true);
}
