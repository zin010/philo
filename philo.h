/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:19:38 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/20 18:43:25 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# define LOCK 1
# define UNLOCK 0

# define NOTHING 0
# define END 1
# define DIED 2

typedef struct s_ref
{
	long	s_sec;
	long	s_usec;
	int		n_of_p;
	int		t_die;
	int		t_eat;
	int		t_sleep;
	int		m_eat;
}			t_ref;

typedef struct s_checklist
{
	int	philo_init;
	int	fork_init;
	int	ate_init;
	int	cnt_init;
	int	flag_init;
	int	*fork_state;
	int	*ate_state;
	int	*cnt_state;
	int	*flag_state;
}		t_checklist;

typedef struct s_philo
{
	pthread_t		me;
	int				num;
	int				flag;
	int				cnt;
	void			*table;
	long			ate_ms;
	long			slept_ms;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*key_ate;
	pthread_mutex_t	*key_cnt;
	pthread_mutex_t	*key_flag;
	t_ref			ref;
}					t_philo;

typedef struct s_table
{
	t_philo			**philo;
	pthread_mutex_t	**fork;
	pthread_mutex_t	**key_ate;
	pthread_mutex_t	**key_cnt;
	pthread_mutex_t	**key_flag;
	t_ref			ref;
	t_checklist		check;
}                   t_table;

#endif