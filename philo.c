/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 17:39:33 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/24 18:33:19 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*nurse(void *pointer)
{
	t_philo	*philo;

	philo = pointer;
	if (philo->eating == 1)
		pthread_mutex_lock(&philo->lock);
		// riorda che devi unlockare il mutex
	if (get_time - philo->eat_time >= philo->die_time)
		philo->status = 1;
	if (philo->eat_cont == philo->max_meals)
		philo->status == 2;
}

void	*routine(void *pointer)
{
	t_philo		*philo;
	pthread_t	tid_supervisor;

	philo = pointer;

	pthread_create(&tid_supervisor, NULL, &nurse, &philo);
	while (philo->status == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		printf("%lu %d has taken a fork\n", get_time, philo->id);
		pthread_mutex_lock(philo->l_fork);
		printf("%lu %d has taken a fork\n", get_time, philo->id);
		philo->eating = 1;
		printf("%lu %d is eating\n", get_time, philo->id);
		philo->eat_cont++;
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		philo->eating = 0;
		printf("%lu %d is sleeping\n", get_time, philo->id);
		ft_usleep(philo->data->time_to_sleep);
		printf("%lu %d is thinking\n", get_time, philo->id);
	}
}

static void	init_philos(t_philo *philo, int i, int argc)
{
	philo->id = i + 1;
	philo->eat_time = philo->data->start_time;
	philo->eat_cont = 0;
	philo->status = 0;
	philo->eating = 0;
	philo->die_time = philo->data->time_to_death;
	if (argc == 6)
		philo->max_meals = philo->data->meals_nb;
	else
		philo->max_meals = -1;
}

static void	init_data(t_data *data, int argc, char **argv)
{
	int				i;
	pthread_mutex_t	mutex;
	pthread_t		tid;

	pthread_mutex_init(&mutex, NULL);
	i = 0;
	data->time_to_death = (u_int64_t) ft_atoi(argv[2]);
	data->time_to_eat = (u_int64_t) ft_atoi(argv[3]);
	data->time_to_sleep = (u_int64_t) ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_nb = (u_int64_t) ft_atoi(argv[5]);
	data->start_time = get_time;
	while (i < data->total_philo)
	{
		init_philos(&data->philos[i], i, argc);
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_create(&tid, NULL, &routine, &data->philos[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	data->total_philo = ft_atoi(argv[1]);
	if (argc == 5 || argc == 6)
		init_data(data, argc, argv);
	else
		write(1, "Input error\n", 12);
	return (0);
}
