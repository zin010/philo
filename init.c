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

int table_init(t_table *t, char **argv)
{
	int			 	i;
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