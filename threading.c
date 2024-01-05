/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:37:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/05 13:45:30 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	wait_threads(t_general *const general)
{
	int	i;

	i = 0;
	while (i <= general->count)
	{
		pthread_join(general->threads[i], NULL);
		i++;
	}
}

/*
	this initializes a fork that is always unusable
	without this the simulation would get stuck if
	these was only one philo.
	An alternative would be to create a hard coded
	situation for that edge case
*/
void	init_fake_fork(t_fork *fake_fork)
{
	fake_fork->used = true;
	pthread_mutex_init(&(fake_fork->mutex), NULL);
	pthread_mutex_init(&(fake_fork->mutex_used), NULL);
}

void	cleanup_fake_fork(t_fork *fake_fork)
{
	pthread_mutex_destroy(&(fake_fork->mutex));
	pthread_mutex_destroy(&(fake_fork->mutex));
}

bool	intit_threading(t_general *const general)
{
	int				i;
	t_philo			*philo;
	pthread_t		*thread;
	t_fork			fake_fork;

	init_fake_fork(&fake_fork);
	general->start_t = get_microseconds();
	general->start_t += general->count * THREADING_INIT_TIME_MICRO;
	i = 0;
	pthread_mutex_lock(&(general->status));
	while (i < general->count)
	{
		philo = general->philos + i;
		thread = general->threads + i;
		philo->start_t = general->start_t;
		if (general->count == 1)
			philo->right_fork = &fake_fork;
		if (pthread_create(thread + 1, NULL, main_loop, philo))
			return (false);
		i++;
	}
	pthread_mutex_unlock(&(general->status));
	wait_threads(general);
	cleanup_fake_fork(&fake_fork);
	return (false);
}
