/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitering.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 16:17:53 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/16 19:22:04 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitering(t_table *t)
{
	int		died;
	int		i;

	died = 0;
	while (!died && !t->flag)
	{
		i = 0;
		while (i < t->n_of_p)
		{
			if (!is_alive(t, t->philo[i]))
			{
				died = i + 1;
				break ;
			}
			if (t->m_eat)
				check_meal_over(t);
			i++;
		}
	}
	usleep(100);
	check_n_cleanup(t, NULL);
	check_n_print(t, died, t->flag);
}

int	is_alive(t_table *t, t_philo *p)//////
{
	long	now;
	long	diff;

	get_now_ms(t, &now);
	pthread_mutex_lock(&t->key);//
	t->check.key_state = LOCK;
	diff = now - p->ate_ms;
	pthread_mutex_unlock(&t->key);//
	t->check.key_state = UNLOCK;
	return (diff < t->t_die);
}

void	check_meal_over(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->n_of_p && t->cnt[i] >= t->m_eat)
		i++;
	if (i == t->n_of_p)
		t->flag = END;
}

void	check_n_print(t_table *t, int d, int n)
{
	long	now;

	get_now_ms(t, &now);
	if (n)
		return ;
	else
		printf("%ld %d is died\n", now, d);
}
