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
    int i;
}