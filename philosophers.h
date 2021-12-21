/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalendi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 17:23:45 by lcalendi          #+#    #+#             */
/*   Updated: 2021/08/16 17:31:56 by lcalendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_const
{
	int	number_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times;
	int	counting;
}	t_const;

typedef struct s_philo
{
	pthread_mutex_t	*forks;
	int				number;
	int				ate;
	double			last_meal;
	t_const			*c;
	int				*ms;
}				t_philo;

typedef struct s_garbage
{
	t_philo			*philos;
	t_const			*c;
	pthread_mutex_t	*forks;
	pthread_t		*tid;
	int				loop;
	int				*ms;
	struct timeval	time;
	struct timeval	real;
}				t_garbage;

int		parsing(t_const *c, int argc, char **argv);
int		init_garbage(t_garbage *norm, int argc, char **argv);
void	init_start_philos(t_garbage *norm);
int		free_all(t_garbage *norm);
int		nalloc(int size, void **pointer);
void	*philosopher(void *rand);

#endif
