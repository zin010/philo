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
	int				l_state;
	int				r_state;
	pthread_mutex_t	*m_ate;
	pthread_mutex_t	*m_cnt;
	pthread_mutex_t	*m_flag;
	t_ref			ref;
}					t_philo;

typedef struct s_table
{
	t_philo			**philo;
	pthread_mutex_t	**fork;
	pthread_mutex_t	**m_ate;
	pthread_mutex_t	**m_cnt;
	pthread_mutex_t	**m_flag;
	int				flag;
	t_ref			ref;
	t_checklist		check;
}					t_table;

void	get_now_ms(long s_sec, long s_usec, long *now_ms);
int		is_white(char c);
int		ft_atoi(char *str);

void	check_n_cleanup(t_table *t, char *s);
void	free_table(t_table *t);
void	free_table_util(t_table *t);

int		init_n_check(t_table *t, char **argv);
int		table_init(t_table *t, char **argv);
int		table_alloc(t_table *t);

int		fork_init(t_table *t);
int		philo_init(t_table *t, t_philo **p);
int		philo_create(t_table *t, t_philo **p, int i);

void	monitering(t_table *t);
int		is_alive(t_table *t, t_philo *p, int i);
void	set_all_flags(t_table *t, t_philo **p);
void	check_meal_over(t_table *t);
void	print_n_cleanup(t_table *t, int died, long d_time);

void	*routine(void *arg);
int		eat(t_philo *p);
int		go_to_sleep(t_philo *p);
int		think(t_philo *p);

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	check_unlockable(t_philo *p);
void	msleep(t_philo *p, int time, long ms);
int		get_flag(t_philo *p);

#endif