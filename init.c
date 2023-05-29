/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 12:48:14 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/29 16:20:37 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->total_philo);
	if (!data->tid)
		return (error("ERROR WHILE ALLOCATING THREADS IDs", data));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->total_philo);
	if (!data->forks)
		return (error("ERROR WHILE ALLOCATING FORKS", data));
	data->philos = malloc(sizeof(t_philo) * data->total_philo);
	if (!data->philos)
		return (error("ERROR WHILE ALLOCATING PHILOS", data));
	return (0);
}

void	init_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].eat_time = data->start_time;
		data->philos[i].eat_cont = 0;
		data->philos[i].eating = 0;
		data->philos[i].die_time = data->time_to_death;
		data->philos[i].max_meals = data->meals_nb;
		pthread_mutex_init(&data->philos[i].lock, NULL);
	}
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 0;
	data->philos[0].l_fork = &data->forks[0];
	data->philos[0].r_fork = &data->forks[data->total_philo - 1];
	i = 1;
	while (i < data->total_philo)
	{
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->total_philo = (int) ft_atoi(argv[1]);
	data->time_to_death = (u_int64_t) ft_atoi(argv[2]);
	data->time_to_eat = (u_int64_t) ft_atoi(argv[3]);
	data->time_to_sleep = (u_int64_t) ft_atoi(argv[4]);
	data->meals_nb = -1;
	if (argc == 6)
		data->meals_nb = (int) ft_atoi(argv[5]);
	if (data->total_philo <= 0 || data->total_philo > 200
		|| data->time_to_death < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (error("INVALID INPUT VALUES", NULL));
	data->start_time = get_time();
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->write, NULL);
	return (0);
}

int	init_threads(t_data *data)
{
	int			i;
	pthread_t	t;

	i = 0;
	if (data->meals_nb > 0)
	{
		if (pthread_create(&t, NULL, &waiter, &data->philos[0]))
			return (error("ERROR WHILE CREATING THREADS", data));
	}
	while (i < data->total_philo)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
			return (error("ERROR WHILE CREATING THREADS", data));
		ft_usleep(1);
		i++;
	}
	i = 0;
	while (i < data->total_philo)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error("ERROR WHILE JOINING THREADS", data));
		i++;
	}
	return (0);
}
