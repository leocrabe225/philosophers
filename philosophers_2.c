/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalendi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 17:20:12 by lcalendi          #+#    #+#             */
/*   Updated: 2021/08/19 17:40:35 by lcalendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	parsing(t_const *c, int argc, char **argv)
{
	int	*pointer;
	int	loop;
	int	leep;

	if (argc < 5 || argc > 6)
		return (1);
	pointer = (void *)c;
	loop = 1;
	c->counting = 0;
	while (loop < argc)
	{
		leep = 0;
		*pointer = 0;
		while (argv[loop][leep] >= '0' && argv[loop][leep] <= '9' && leep < 6)
			*pointer = *pointer * 10 + argv[loop][leep++] - '0';
		if (argv[loop][leep] || leep == 6 || *pointer == 0)
			return (1);
		pointer += 1;
		loop++;
	}
	if (argc == 6)
		c->counting = 1;
	else
		c->times = 1;
	return (0);
}

int	init_garbage(t_garbage *norm, int argc, char **argv)
{
	if (nalloc(sizeof(t_const), (void **)&norm->c))
		return (1);
	if (parsing(norm->c, argc, argv))
		return (1);
	if (nalloc(sizeof(int), (void **)&norm->ms))
		return (1);
	if (nalloc(sizeof(pthread_mutex_t) * norm->c->number_philo,
			(void **)&norm->forks))
		return (1);
	if (nalloc(sizeof(norm->tid) * norm->c->number_philo, (void **)&norm->tid))
		return (1);
	if (nalloc(sizeof(t_philo) * norm->c->number_philo, (void **)&norm->philos))
		return (1);
	return (0);
}

void	init_start_philos(t_garbage *norm)
{	
	norm->loop = 0;
	while (norm->loop < norm->c->number_philo)
	{
		norm->philos[norm->loop].number = norm->loop;
		norm->philos[norm->loop].ate = 0;
		norm->philos[norm->loop].c = norm->c;
		norm->philos[norm->loop].forks = norm->forks;
		norm->philos[norm->loop].ms = norm->ms;
		pthread_mutex_init(&(norm->forks[norm->loop]), 0);
		norm->loop++;
	}
	norm->loop = norm->c->number_philo - 1;
	while (norm->loop >= 0)
	{
		gettimeofday(&norm->real, 0);
		norm->philos[norm->loop].last_meal = (double)(norm->real.tv_sec) + (
				((double)norm->real.tv_usec) / 1000000);
		pthread_create(&(norm->tid[norm->loop]), NULL,
			*philosopher, &(norm->philos[norm->loop]));
		usleep(200);
		norm->loop--;
	}
}

int	free_all(t_garbage *norm)
{
	int	loop;

	loop = 0;
	while (loop < norm->c->number_philo)
		pthread_mutex_destroy(&(norm->forks[loop++]));
	free(norm->c);
	free(norm->ms);
	free(norm->forks);
	free(norm->tid);
	free(norm->philos);
	return (0);
}
