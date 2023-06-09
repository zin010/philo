/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:15:34 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/16 19:17:28 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	memset((void *)&table, 0, sizeof(t_table));
	if (argc < 5)
	{
		printf("arguments error\n");
		return (EXIT_FAILURE);
	}
	if (!init_n_check(&table, argv))
		return (EXIT_FAILURE);
	monitering(&table);
	return (EXIT_SUCCESS);
}

void	check_n_cleanup(t_table *t, char *s)///;
{
	int	i;

	i = 0;
	if (t->check.key_init && t->check.key_state)
		pthread_mutex_unlock(&t->key);
	while (t->check.fork_state && i < t->n_of_p)
	{
		if (t->check.fork_state[i])
			pthread_mutex_unlock(t->fork[i]);
		i++;
	}
	i = 0;
	while (t->philo && i < t->check.philo_init)
		pthread_join(t->philo[i++]->me, NULL);
	i = 0;
	while (t->fork && i < t->check.fork_init)
		pthread_mutex_destroy(t->fork[i++]);
	if (t->check.key_init)
		pthread_mutex_destroy(&t->key);
	free_table(t);
	if (s)
		printf("%s\n", s);
}

void	free_table(t_table *t)
{
	int	i; 

	i = 0;
	while (t->philo && i < t->check.philo_init)
		free(t->philo[i++]);
	i = 0;
	while (t->fork && i < t->check.fork_init)
		free(t->fork[i++]);
	if (t->philo)
		free(t->philo);
	if (t->fork)
		free(t->fork);
	if (t->check.fork_state)
		free(t->check.fork_state);
	if (t->cnt)
		free(t->cnt);
}

int	is_white(char c)
{
	if (c == '\f' || c == '\v' || c == '\r'
		|| c == '\t' || c == '\n' || c == ' ')
		return (true);
	return (false);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	while (is_white(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] == '0')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		res = 10 * res + (str[i++] - '0');
	return (res * sign);
}
