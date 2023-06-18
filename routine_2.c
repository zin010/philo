/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:48:42 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/18 12:54:20 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	t_table	*t;
	int		right_handed;
	long	now;

	t = (t_table *)p->table;
	right_handed = p->num % 2;
	pthread_mutex_lock(f);
	record_fork_lock(p, right_handed, 'f', t);
	pthread_mutex_lock(s);
	record_fork_lock(p, right_handed, 's', t);
	get_now_ms(p->table, &now);
	if (!t->flag)
		printf("%ld %d has taken a fork\n", now, p->num);
}

void	record_fork_lock(t_philo *p, int right_handed, char c, t_table *t)///;
{
	if (c == 'f')
	{
		if (right_handed)
		{
			if (p->num == 1)
				t->check.fork_state[t->n_of_p - 1] = LOCK;
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
				t->check.fork_state[t->n_of_p - 1] = LOCK;
			else
				t->check.fork_state[p->num - 2] = LOCK;
		}
	}
}

void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	t_table	*t;
	int		right_handed;

	t = (t_table *)p->table;
	right_handed = p->num % 2;
	pthread_mutex_unlock(f);
	record_fork_unlock(p, right_handed, 'f', t);
	pthread_mutex_unlock(s);
	record_fork_unlock(p, right_handed, 's', t);
}

void	record_fork_unlock(t_philo *p, int right_handed, char c, t_table *t)
{
	if (c == 'f')
	{
		if (right_handed)
		{
			if (p->num == 1)
				t->check.fork_state[t->n_of_p - 1] = UNLOCK;
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
				t->check.fork_state[t->n_of_p - 1] = UNLOCK;
			else
				t->check.fork_state[p->num - 2] = UNLOCK;
		}
	}
}
