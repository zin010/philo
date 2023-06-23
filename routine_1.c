/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:45:35 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:53:25 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	while (!get_flag(p))
	{
		if (p->num % 2)
			fork_lock_n_check(p->r_fork, p->l_fork, p);
		else
		{
			usleep(10);
			fork_lock_n_check(p->l_fork, p->r_fork, p);
		}
		if (!eat(p))
			break ;
		if (get_flag(p))
			break ;
		if (!go_to_sleep(p))
			break ;
		if (!think(p))
			break ;
	}
	check_unlockable(p);
	return (NULL);
}

int	eat(t_philo *p)
{
	long	ms;

	if (get_flag(p))
		return (false);
	else
	{
		pthread_mutex_lock(p->m_ate);
		get_now_ms(p->ref.s_sec, p->ref.s_usec, &(p->ate_ms));
		ms = p->ate_ms;
		pthread_mutex_unlock(p->m_ate);
		if (!get_flag(p))
			printf("%ld %d is eating\n", ms, p->num);
		else
			return (false);
		msleep(p, p->ref.t_eat, ms);
		pthread_mutex_lock(p->m_cnt);
		p->cnt++;
		pthread_mutex_unlock(p->m_cnt);
		if (p->num % 2)
			fork_unlock_n_check(p->r_fork, p->l_fork, p);
		else
			fork_unlock_n_check(p->l_fork, p->r_fork, p);
	}
	return (true);
}

int	go_to_sleep(t_philo *p)
{
	if (get_flag(p))
		return (false);
	get_now_ms(p->ref.s_sec, p->ref.s_usec, &(p->slept_ms));
	if (get_flag(p))
		return (false);
	else
		printf("%ld %d is sleeping\n", p->slept_ms, p->num);
	msleep(p, p->ref.t_sleep, p->slept_ms);
	return (true);
}

int	think(t_philo *p)
{
	long	now;

	if (get_flag(p))
		return (false);
	get_now_ms(p->ref.s_sec, p->ref.s_usec, &now);
	if (get_flag(p))
		return (false);
	else
		printf("%ld %d is thinking\n", now, p->num);
	usleep(500);
	return (true);
}
