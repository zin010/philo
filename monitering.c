/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitering.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:17:01 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:47:51 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitering(t_table *t)
{
	long	d_time;
	int		died;
	int		i;

	died = 0;
	while (!died && !t->flag)
	{
		i = 0;
		while (i < t->ref.n_of_p)
		{
			if (!is_alive(t, t->philo[i], i))
			{
				died = i + 1;
				t->flag = DIED;
				set_all_flags(t, t->philo);
				get_now_ms(t->ref.s_sec, t->ref.s_usec, &d_time);
				break ;
			}
			if (t->ref.m_eat)
				check_meal_over(t);
			i++;
		}
	}
	print_n_cleanup(t, died, d_time);
}

int	is_alive(t_table *t, t_philo *p, int i)
{
	long	now;
	long	diff;

	get_now_ms(t->ref.s_sec, t->ref.s_usec, &now);
	pthread_mutex_lock(t->m_ate[i]);
	diff = now - p->ate_ms;
	pthread_mutex_unlock(t->m_ate[i]);
	return (diff <= t->ref.t_die);
}

void	set_all_flags(t_table *t, t_philo **p)
{
	int	i;

	i = 0;
	while (i < t->ref.n_of_p)
	{
		pthread_mutex_lock(t->m_flag[i]);
		p[i]->flag = t->flag;
		pthread_mutex_unlock(t->m_flag[i]);
		i++;
	}
}

void	check_meal_over(t_table *t)
{
	int	i;
	int	over;

	over = 0;
	i = 0;
	while (!over && i < t->ref.n_of_p)
	{
		pthread_mutex_lock(t->m_cnt[i]);
		if (t->philo[i]->cnt < t->ref.m_eat)
			over = 1;
		pthread_mutex_unlock(t->m_cnt[i]);
		i++;
	}
	if (i == t->ref.n_of_p)
	{
		t->flag = END;
		set_all_flags(t, t->philo);
	}
}

void	print_n_cleanup(t_table *t, int died, long d_time)
{
	if (t->flag != END)
		printf("%ld %d died\n", d_time, died);
	check_n_cleanup(t, NULL);
}
