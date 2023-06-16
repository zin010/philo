/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 16:05:40 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/16 18:12:35 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_flag_n_usleep(t_table *t, int n)
{
	t->flag = n;
	usleep(100);
}

int	get_now_ms(t_table *t, long *now_ms)///;
{
	struct timeval	now;

	if (gettimeofday(&now, NULL))
		return (false);
	*now_ms = (now.tv_sec - t->start.tv_sec) * 1000;
	*now_ms = *now_ms + (now.tv_usec - t->start.tv_usec) / 1000;
	return (true);
}

void	msleep(t_table *t, t_philo *p, int m, char c)
{
	long	now;
	int		i;

	i = 0;
	while (true)
	{
		get_now_ms(t, &now);
		if (c == 'e' && (now - p->ate_ms) >= m)
			break ;
		if (c == 's' && (now - p->slept_ms) >= m)
			break ;
		usleep(100);
	}
}
