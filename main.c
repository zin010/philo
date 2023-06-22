/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:17:01 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:47:51 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5)
	{
		printf("arguments error\n");
		return (EXIT_FAILURE);
	}
	memset((void *)&table, 0, sizeof(t_table));
	if (!init_n_check(&table, argv))
		return (EXIT_FAILURE);
	monitering(t);
	return (EXIT_SUCCESS);
}

void	get_now_ms(long s_sec, long s_usec, long *now_ms)
{
	struct timeval	now;

	gettimeofday(&now, NULL)
	*now_ms = (now.tv_sec - s_sec) * 1000 + (now.tv_usec - s_usec) / 1000;
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
