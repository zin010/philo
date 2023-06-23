/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:45:35 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:53:25 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	int		right_handed;
	long	now;

	right_handed = p->num % 2;
	pthread_mutex_lock(f);
	p->l_state = LOCK;
	pthread_mutex_lock(s);
	p->r_state = LOCK;
	get_now_ms(p->ref.s_sec, p->ref.s_usec, &now);
	if (!p->flag)
		printf("%ld %d has taken a fork\n", now, p->num);
}

void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p)
{
	int	right_handed;

	right_handed = p->num % 2;
	pthread_mutex_unlock(f);
	p->l_state = UNLOCK;
	pthread_mutex_unlock(s);
	p->r_state = UNLOCK;
}

void	check_unlockable(t_philo *p)
{
	if (p->l_state)
		pthread_mutex_unlock(p->l_fork);
	if (p->r_state)
		pthread_mutex_unlock(p->r_fork);
}

void	msleep(t_philo *p, int time, long ms)
{
	long	now;
	int		i;

	i = 0;
	while (true)
	{
		get_now_ms(p->ref.s_sec, p->ref.s_usec, &now);
		if ((now - ms) >= time)
			break ;
		usleep(100);
	}
}

int	get_flag(t_philo *p)
{
	int	f;

	pthread_mutex_lock(p->m_flag);
	f = p->flag;
	pthread_mutex_unlock(p->m_flag);
	return (f);
}
