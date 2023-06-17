/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:32:34 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/17 17:44:31 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		{
			usleep(10);
			fork_lock_n_check(p->l_fork, p->r_fork, p);
		}
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

int	eat(t_philo *p, t_table *t)///;
{
	if (t->flag)
		return (false);
	get_now_ms(t, &(p->ate_ms));
	if (t->flag)
		return (false);
	printf("%ld %d is eating\n", p->ate_ms, p->num);
//printf("\t\t\t%d eat %d times\n", p->num, t->cnt[p->num - 1] + 1);
	msleep(t, p, t->t_eat, 'e');
	t->cnt[p->num - 1]++;
	if (p->num % 2)
		fork_unlock_n_check(p->r_fork, p->l_fork, p);
	else
		fork_unlock_n_check(p->l_fork, p->r_fork, p);
	return (true);
}

int	go_to_sleep(t_philo *p, t_table *t)
{
	if (t->flag)
		return (false);
	get_now_ms(t, &(p->slept_ms));
	if (t->flag)
		return (false);
	printf("%ld %d is sleeping\n", p->slept_ms, p->num);
	msleep(t, p, t->t_sleep, 's');
	return (true);
}

int	think(t_philo *p, t_table *t)
{
	long	now;

	if (t->flag)
		return (false);
	get_now_ms(t, &now);
	if (t->flag)
		return (false);
	printf("%ld %d is thinking\n", now, p->num);
	usleep(500);
	return (true);
}
