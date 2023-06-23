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

int	fork_init(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->ref.n_of_p)
	{
		t->fork[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		t->m_ate[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		t->m_cnt[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		t->m_flag[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!t->fork[i] || !t->m_ate[i] || !t->m_cnt[i] || !t->m_flag[i])
			return (false);
		if (pthread_mutex_init(t->fork[i], NULL)
			|| pthread_mutex_init(t->m_ate[i], NULL)
			|| pthread_mutex_init(t->m_cnt[i], NULL)
			|| pthread_mutex_init(t->m_flag[i], NULL))
			return (false);
		t->check.fork_init++;
		t->check.ate_init++;
		t->check.cnt_init++;
		t->check.flag_init++;
		i++;
	}
	return (true);
}

int	philo_init(t_table *t, t_philo **p)
{
	int	i;

	i = 0;
	while (i < t->ref.n_of_p)
	{
		p[i] = (t_philo *)malloc(sizeof(t_philo));
		if (!p[i])
			return (false);
		p[i]->num = i + 1;
		p[i]->flag = NOTHING;
		p[i]->cnt = 0;
		p[i]->table = (void *)t;
		p[i]->ate_ms = 0;
		p[i]->slept_ms = 0;
		p[i]->l_state = UNLOCK;
		p[i]->r_state = UNLOCK;
		p[i]->l_fork = t->fork[i];
		p[i]->m_ate = t->m_ate[i];
		p[i]->m_cnt = t->m_cnt[i];
		p[i]->m_flag = t->m_flag[i];
		i++;
	}
	if (!philo_create(t, p, 0))
		return (false);
	return (true);
}

int	philo_create(t_table *t, t_philo **p, int i)
{
	while (i < t->ref.n_of_p)
	{
		p[i]->ref.s_sec = t->ref.s_sec;
		p[i]->ref.s_usec = t->ref.s_usec;
		p[i]->ref.n_of_p = t->ref.n_of_p;
		p[i]->ref.t_die = t->ref.t_die;
		p[i]->ref.t_eat = t->ref.t_eat;
		p[i]->ref.t_sleep = t->ref.t_sleep;
		p[i]->ref.m_eat = t->ref.m_eat;
		i++;
	}
	while (--i > 0)
	p[i]->r_fork = t->fork[i - 1];
	p[0]->r_fork = t->fork[t->ref.n_of_p - 1];
	while (i < t->ref.n_of_p)
	{
		if (pthread_create(&p[i]->me, NULL, routine, (void *)p[i]))
			return (false);
		t->check.philo_init++;
		i++;
	}
	return (true);
}
