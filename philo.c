/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 17:39:33 by gpecci            #+#    #+#             */
/*   Updated: 2023/05/26 20:17:42 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	set_alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->total_philo);
	if (!data->tid)
		return (error("ERROR WHILE ALLOCATING THREADS IDs", data));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->total_philo);
	if (!data->forks)
		return (error("ERROR WHILE ALLOCATING PHILOS", data));
	data->philos = malloc(sizeof(t_philo) * data->total_philo);
	if (!data->philos)
		return (error("ERROR WHILE ALLOCATING FORKS", data));
	return (0);
}

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
	pthread_t	tid_supervisor;

	philo = (t_philo *)pointer;
	pthread_create(&tid_supervisor, NULL, &waiter, &philo);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		message("has taken a fork", philo);
		pthread_mutex_lock(philo->l_fork);
		message("has taken a fork", philo);
		philo->eating = 1;
		philo->eat_time = get_time();
		message("is eating", philo);
		philo->eat_cont++;
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		philo->eating = 0;
		message("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
		message("is thinking", philo);
	}
	return ((void *)0);
}

static void	init_philos(t_data *data, int argc)
{
	int	i;

	i = -1;
	while(++i < data->total_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eat_time = data->start_time;
		data->philos[i].eat_cont = 0;
		data->philos[i].status = 0;
		data->philos[i].eating = 0;
		data->philos[i].die_time = data->time_to_death;
		data->philos[i].max_meals = -1;
		if (argc == 6)
			data->philos[i].max_meals = data->meals_nb;
		pthread_mutex_init(&data->philos[i].lock, NULL);
	}
}

static int	init_forks(t_data *data)
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

static int	init_data(t_data *data, int argc, char **argv)
{
	int				i;
	pthread_t		tid;

	i = 0;
	data->time_to_death = (u_int64_t) ft_atoi(argv[2]);
	data->time_to_eat = (u_int64_t) ft_atoi(argv[3]);
	data->time_to_sleep = (u_int64_t) ft_atoi(argv[4]);
	data->meals_nb = -1;
	if (argc == 6)
		data->meals_nb = (u_int64_t) ft_atoi(argv[5]);
	data->start_time = get_time();
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->write, NULL);
	set_alloc(data);
	init_forks(data);
	init_philos(data, argc);
	if (data->meals_nb != -1)
	{
		if (pthread_create(&tid, NULL, &waiter, &data->philos[0]))
			return (error("ERROR WHILE CREATING THREADS", data));
	}
	while (i < data->total_philo)
	{
		pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]);
		ft_usleep(1);
		i++;
	}
	i = 0;
	while (i < data->total_philo)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error("ERROR WHILE JOINING THREADS", data));
	}
	return (0);
}


int	main(int argc, char **argv)
{
	t_data	data;

	data.total_philo = ft_atoi(argv[1]);
	if (argc == 5 || argc == 6)
	{
		init_data(&data, argc, argv);
		ft_exit(&data);
	}
	else
		write(1, "Input error\n", 12);
	return (0);
}
