/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:07:41 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/26 19:58:25 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# define TAKE_FORKS "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "died"

struct	s_data;

typedef struct s_philo
{
	struct s_data	*data;
	int				id;
	u_int64_t		die_time;
	u_int64_t		eat_time;
	u_int64_t		sleep_time;
	int				eat_cont;
	int				status;
	int				max_meals;
	int				eating;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}	t_philo;

typedef struct s_data
{
	pthread_t		*tid;
	int				total_philo;
	int				meals_nb;
	int				dead;
	int				finished;
	u_int64_t		time_to_death;
	u_int64_t		time_to_eat;
	u_int64_t		time_to_sleep;
	u_int64_t		start_time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_data;

int			ft_atoi(const char *str);
u_int64_t	get_time(void);
int			ft_usleep(useconds_t time);
void		*routine(void *data_pointer);
void		*nurse(void *pointer);
void		*waiter(void *food);
void		ft_exit(t_data *data);
int			error(char *str, t_data *data);
void		clear_data(t_data *data);
void		message(char *str, t_philo *philo);

#endif
