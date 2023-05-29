/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 17:39:33 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/29 13:04:29 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*waiter(void *food)
{
	t_philo	*philo;

	philo = (t_philo *) food;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->total_philo)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*nurse(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *) pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if ((get_time() - philo->eat_time >= philo->die_time) && philo->eating == 0)
		{
			philo->status = 1;
			message("died", philo);
			philo->data->dead = 1;
		}
		if (philo->eat_cont == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*routine(void *pointer)
{
	t_philo		*philo;

	philo = (t_philo *)pointer;
	pthread_create(&philo->t1, NULL, &nurse, &philo);
	while (philo->data->dead == 0)
	{
		eating(philo);
		message("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
		message("is thinking", philo);
	}
	if (pthread_join(philo->t1, NULL))
		return ((void *)1);
	return ((void *)0);
}

void	init(t_data *data, int argc, char **argv)
{
	init_data(data, argc, argv);
	set_alloc(data);
	init_forks(data);
	init_philos(data, argc);
	init_threads(data);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		init(&data, argc, argv);
		ft_exit(&data);
	}
	else
		write(1, "Input error\n", 12);
	return (0);
}
