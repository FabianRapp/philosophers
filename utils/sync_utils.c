/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 07:30:05 by fabi              #+#    #+#             */
/*   Updated: 2024/01/03 17:30:47 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t	get_microseconds_sync(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

static inline int64_t	death_time_millisec(t_philo *philo)
{
	return ((philo->current_t - philo->start_t) / MICROSEC_TO_MILLISEC);
}

static inline int64_t	cur_time_millisec(t_philo *philo)
{
	return ((philo->current_t - philo->start_t) >> SHIFT_DIV_ESTIMATE);
}

static inline void	kill_philo(t_philo *philo)
{
	printf("%ld %u died\n", death_time_millisec(philo), philo->index);
	*(philo->exit) = true;
	pthread_mutex_unlock(philo->status);
}

bool	check_exit(t_philo *philo)
{
	int64_t	local_current_t;

	local_current_t = get_microseconds_sync();
	usleep(1);
	pthread_mutex_lock(philo->status);
	if (!(*(philo->exit)))
	{
		if ((local_current_t <= philo->death_t))
		{
			pthread_mutex_unlock(philo->status);
			return (false);
		}
		else
		{
			if (*(philo->exit))
			{
				pthread_mutex_unlock(philo->status);
				return (true);
			}
			//philo->current_t = get_microseconds_sync();
			philo->current_t = local_current_t;
			kill_philo(philo);
			return (true);
		}
	}
	pthread_mutex_unlock(philo->status);
	return (true);
}

bool	print_status(t_philo *philo, char *status)
{
	int64_t	local_current_time;

	local_current_time = get_microseconds_sync();
	pthread_mutex_lock(philo->status);
	if (!(*(philo->exit)))
	{
		if ((local_current_time <= philo->death_t))
		{
			printf("%ld %u %s\n", cur_time_millisec(philo), philo->index, status);
			pthread_mutex_unlock(philo->status);
			return (true);
		}
		else
		{
			if (*(philo->exit))
			{
				pthread_mutex_unlock(philo->status);
				return (true);
			}
			philo->current_t = local_current_time;
			kill_philo(philo);
			return (false);
		}
	}
	pthread_mutex_unlock(philo->status);
	return (false);
}
