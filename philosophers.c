/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalendi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:22:59 by lcalendi          #+#    #+#             */
/*   Updated: 2021/08/16 17:37:08 by lcalendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	nalloc(int size, void **pointer)
{
	*pointer = malloc(size);
	if (*pointer)
		return (0);
	else
		return (1);
}

void	magic_wait(double time)
{
	struct timeval	start_time;
	struct timeval	new_time;

	gettimeofday(&start_time, 0);
	while (1)
	{
		usleep(50);
		gettimeofday(&new_time, 0);
		if ((double)(new_time.tv_sec - start_time.tv_sec) +
	((double)new_time.tv_usec - (double)start_time.tv_usec) / 1000000 > time)
			return ;
	}
}

void	*philosopher(void *rand)
{
	t_philo			*me;
	struct timeval	time;

	me = rand;
	while (1)
	{
		pthread_mutex_lock(&(me->forks[me->number]));
		printf("%d %d has taken a fork\n", *(me->ms), me->number + 1);
		pthread_mutex_lock(&(me->forks[(me->number - 1
					+ me->c->number_philo) % me->c->number_philo]));
		printf("%d %d has taken a fork\n", *(me->ms), me->number + 1);
		magic_wait((double)me->c->time_to_eat / 1000);
		if (me->c->counting)
			me->ate++;
		printf("%d %d is sleeping\n", *(me->ms), me->number + 1);
		gettimeofday(&time, 0);
		me->last_meal = (double)(time.tv_sec) + (((double)time.tv_usec)
				/ 1000000);
		pthread_mutex_unlock(&(me->forks[me->number]));
		pthread_mutex_unlock(&(me->forks[(me->number - 1
					+ me->c->number_philo) % me->c->number_philo]));
		magic_wait((double)me->c->time_to_sleep / 1000);
		printf("%d %d is thinking\n", *(me->ms), me->number + 1);
	}
	return (0);
}

int	death_check(t_garbage *norm)
{
	while (norm->loop != norm->c->number_philo)
	{
		if (norm->philos[norm->loop++].last_meal + \
	((double)norm->c->time_to_die) / 1000 < (double)(norm->time.tv_sec) + \
				(((double)norm->time.tv_usec) / 1000000))
		{
			printf("%d %d died\n", *norm->ms, norm->loop);
			norm->loop = 0;
			while (norm->loop != norm->c->number_philo)
				pthread_detach(norm->tid[norm->loop++]);
			return (1);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_garbage	norm;	

	if (init_garbage(&norm, argc, argv))
		return (1);
	init_start_philos(&norm);
	while (1)
	{
		gettimeofday(&norm.time, 0);
		norm.loop = 0;
		*(norm.ms) = ((double)(norm.time.tv_sec - norm.real.tv_sec) + \
	((double)norm.time.tv_usec - (double)norm.real.tv_usec) / 1000000) * 1000;
		if (death_check(&norm))
			return (free_all(&norm));
		norm.loop = 0;
		while (norm.philos[norm.loop].ate >= norm.c->times && \
				norm.loop < norm.c->number_philo)
			norm.loop++;
		if (norm.loop == norm.c->number_philo)
		{
			norm.loop = 0;
			while (norm.loop != norm.c->number_philo)
				pthread_detach(norm.tid[norm.loop++]);
			return (free_all(&norm));
		}
	}
}
