/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 17:39:33 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/29 16:20:52 by gpecci           ###   ########.fr       */
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
			philo->data->dead = 1;
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
		if (((get_time() - philo->eat_time) >= philo->die_time)
			&& philo->eating == 0)
			message("died", philo);
		if (philo->eat_cont == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->eat_cont++;
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
	if (pthread_create(&philo->t1, NULL, &nurse, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eating(philo);
		message("is thinking", philo);
	}
	if (pthread_join(philo->t1, NULL))
		return ((void *)1);
	return ((void *)0);
}

int	init(t_data *data, int argc, char **argv)
{
	if (init_data(data, argc, argv))
		return (1);
	if (set_alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_philos(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("NEED 4 OR 5 ARGUMENTS\n");
		return (1);
	}
	if (input_checker(argv))
		return (1);
	if (init(&data, argc, argv))
		return (1);
	if (data.total_philo == 1)
		return (case_one(&data));
	if (init_threads(&data))
		return (1);
	ft_exit(&data);
	return (0);
}
