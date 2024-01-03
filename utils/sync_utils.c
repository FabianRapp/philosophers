/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 07:30:05 by fabi              #+#    #+#             */
/*   Updated: 2023/12/29 19:55:11 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline void	kill_philo(t_philo *philo)
{
	printf("%ld %d died\n", philo->current_time - philo->total_start_t, philo->index);
	printf("next eat: %ld\n", (philo->next_eat - philo->total_start_t));
	printf("death time: %ld\n", philo->death_time - philo->total_start_t);
	*(philo->exit) = true;
	pthread_mutex_unlock(philo->mutex_exit);
}

bool	check_exit(t_philo *philo)
{
	int64_t	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{
		if (!(local_current_time > philo->death_time))
		{
			pthread_mutex_unlock(philo->mutex_exit);
			return (false);
		}
		else
		{
			philo->current_time = local_current_time;
			kill_philo(philo);
			return (true);
		}
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (true);
}

bool	print_status(t_philo *philo, char *status)
{
	int64_t	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{

		if ((local_current_time <= philo->death_time))
		{
			printf("%ld %d %s\n", (philo->current_time - philo->total_start_t), philo->index, status);
			pthread_mutex_unlock(philo->mutex_exit);
			return (true);
		}
		else
		{
			philo->current_time = local_current_time;
			kill_philo(philo);
			return (false);
		}
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (false);
}
