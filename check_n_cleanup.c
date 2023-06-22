/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_n_clean_up.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:17:01 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:47:51 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_n_cleanup(t_table *t, char *s, int i)
{
	while (i < t->ref.n_of_p)
	{
		if (t->check.fork_state && t->check.fork_state[i])
			pthread_mutex_unlock(t->fork[i]);
		if (t->check.ate_state && t->check.ate_state[i])
			pthread_mutex_unlock(t->m_ate[i]);
		if (t->check.cnt_state && t->check.cnt_state[i])
			pthread_mutex_unlock(t->m_cnt[i]);
		if (t->check.flag_state && t->check.flag_state[i])
			pthread_mutex_unlock(t->m_flag[i]);
		if (t->philo && i < t->check.philo_init)
			pthread_join(t->philo[i]->me, NULL);
		if (t->fork && i < t->check.fork_init)
			pthread_mutex_destroy(t->fork[i]);
		if (t->m_ate && i < t->check.ate_init)
			pthread_mutex_destroy(t->m_ate[i]);
		if (t->m_cnt && i < t->check.cnt_init)
			pthread_mutex_destroy(t->m_cnt[i]);
		if (t->m_flag && i < t->check.flag_init)
			pthread_mutex_destroy(t->m_flag[i]);
		i++;
	}
	free_table(t);
	if (s)
		printf("%s\n", s);
}

void	free_table(t_table *t)
{
	free_table_util(t);
	if (t->philo)
		free(t->philo);
	if (t->fork)
		free(t->fork);
	if (t->m_ate)
		free(t->m_ate);
	if (t->m_cnt)
		free(t->m_cnt);
	if (t->m_flag)
		free(t->m_flag);
	if (t->check.fork_state)
		free(t->check.fork_state);
	if (t->check.ate_state)
		free(t->check.ate_state);
	if (t->check.cnt_state)
		free(t->check.cnt_state);
	if (t->check.flag_state)
		free(t->check.flag_state);
}

void	free_table_util(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->ref.n_of_p)
	{
		if (t->philo && i < t->check.philo_init)
			free(t->philo[i]);
		if (t->fork && i < t->check.fork_init)
			free(t->fork[i]);
		if (t->m_ate && i < t->check.ate_init)
			free(t->m_ate[i]);
		if (t->m_cnt && i < t->check.cnt_init)
			free(t->m_cnt[i]);
		if (t->m_flag && i < t->check.flag_init)
			free(t->m_flag);
		i++;
	}
}
