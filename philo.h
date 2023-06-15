/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikang2 <jikang2@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:05:18 by jikang2           #+#    #+#             */
/*   Updated: 2023/06/15 16:48:04 by jikang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# define LOCK 1
# define UNLOCK 0

# define NOTHING 0
# define END 1
# define EXTERNAL 2
# define INTERNAL 3

typedef struct s_checklist
{
	int	fork_init;
	int	philo_init;
	int	*fork_state;
}		t_checklist;

typedef struct s_philo
{
	pthread_t		me;
	void			*table;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	long			ate_ms;
	long			slept_ms;
	int				num;
}					t_philo;

typedef struct s_table
{
	t_philo			**philo;
	pthread_mutex_t	**fork;
	int				*cnt;
	struct timeval	start;
	int				n_of_p;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				m_eat;
	int				flag;
	t_checklist		check;
}					t_table;

void	check_n_cleanup(t_table *t, char *s);
void	free_table(t_table *t);
int		is_white(char c);
int		ft_atoi(char *str);

int		init_n_check(t_table *t, char **argv);
int		table_init(t_table *t, char **argv);
int		fork_init(t_table *t);
int		philo_init(t_table *t, t_philo **p);
int		philo_create(t_table *t, t_philo **p);

void	*routine(void *arg);
int		eat(t_philo *p, t_table *t);
int		go_to_sleep(t_philo *p, t_table *t);
int		think(t_philo *p, t_table *t);

void	fork_lock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	record_fork_lock(t_philo *p, int right_handed, char c, t_table *t);
void	fork_unlock_n_check(pthread_mutex_t *f, pthread_mutex_t *s, t_philo *p);
void	record_fork_unlock(t_philo *p, int right_handed, char c, t_table *t);

void	set_flag_n_usleep(t_table *t, int n);
int		get_now_ms(t_table *t, long *now_ms);
void	msleep(t_table *t, t_philo *p, int m, char c);

void	monitering(t_table *t);
int		is_alive(t_philo *p);
void	check_meal_over(t_table *t);
void	check_n_print(t_table *t, int d, int n);

#endif
