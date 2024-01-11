/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 07:30:05 by fabi              #+#    #+#             */
/*   Updated: 2024/01/12 00:48:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// performence critical
static inline int64_t __attribute__((always_inline))
	get_microseconds_sync(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

// this function is responsible for the mutex unlock
// not performance critical
static void	do_exit(t_philo *restrict const philo, const bool locked_mutex)
{
	int64_t	death_time;

	if (!locked_mutex)
		pthread_mutex_lock(philo->status_mutex_ptr);
	if (!(*(philo->exit)))
	{
		death_time = (get_microseconds_sync() - philo->start_t)
			* MICROSEC_TO_MILLISEC_FACTOR;
		//usleep(20000);
		//printf("%lld %lld died\n", death_time,  philo->index);
		*(philo->output_size) += put_output_to_buffer(get_microseconds_sync() - philo->start_t, philo->index, philo->output_buffer + *(philo->output_size), " died\n");
		*(philo->exit) = true;
	}
	pthread_mutex_unlock(philo->status_mutex_ptr);
}

// performance critical in forks and death loop
bool	is_dead(t_philo *restrict const philo)
{
	if (philo->death_t >= get_microseconds())
	{
		return (false);
	}
	do_exit(philo, false);
	return (true);
}

// performence critical
bool	change_status(t_philo *restrict const philo, const char *restrict status)
{
	int64_t			local_current_t;
	bool			*local_exit_ptr;
	int64_t			local_death_t;
	pthread_mutex_t	*local_mutex_ptr;

	local_death_t = philo->death_t;
	local_exit_ptr = philo->exit;
	local_mutex_ptr = philo->status_mutex_ptr;
	pthread_mutex_lock(local_mutex_ptr);
	philo->current_t = get_microseconds_sync();
	local_current_t = philo->current_t;
	if (!(*local_exit_ptr))
	{
		if (local_death_t >= local_current_t)
		{
			*(philo->output_size) += put_output_to_buffer((local_current_t - philo->start_t), philo->index, philo->output_buffer + *(philo->output_size), status);
			pthread_mutex_unlock(local_mutex_ptr);
			return (true);
		}
	}
	do_exit(philo, true);
	return (false);
}
