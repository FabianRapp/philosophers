/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 07:30:05 by fabi              #+#    #+#             */
/*   Updated: 2024/01/08 18:35:00 by frapp            ###   ########.fr       */
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

// performence critical
static inline int64_t	cur_time_millisec(t_philo *restrict const philo)
{
	return ((philo->current_t - philo->start_t)
		* MICROSEC_TO_MILLISEC_FACTOR);
}

// // this gives better performence but
//  // gives slightly inaccurate timestamp outputs
// static inline int64_t	cur_time_millisec(t_philo *restrict const philo)
// {
// 	return ((philo->current_t - philo->start_t) >> SHIFT_DIV_ESTIMATE);
// }

// this function is responsible for the mutex unlock
// not performance critical
static void	do_exit(t_philo *restrict const philo, const bool locked_mutex)
{
	int64_t	death_time;

	if (!locked_mutex)
		pthread_mutex_lock(philo->status);
	if (!(*(philo->exit)))
	{
		philo->current_t = get_microseconds_sync();
		death_time = (philo->current_t - philo->start_t)
			* MICROSEC_TO_MILLISEC_FACTOR;
		printf("%lld %lld died\n", death_time, philo->index);
		*(philo->exit) = true;
	}
	pthread_mutex_unlock(philo->status);
}

// performence critical
bool	change_status(t_philo *restrict const philo, const char *restrict const status)
{
	int64_t			local_current_t;
	bool			*local_exit_ptr;
	int64_t			local_death_t;
	pthread_mutex_t	*local_mutex_ptr;

	local_death_t = philo->death_t;
	local_exit_ptr = philo->exit;
	local_mutex_ptr = philo->status;
	local_current_t = get_microseconds_sync();
	pthread_mutex_lock(local_mutex_ptr);
	if (!(*local_exit_ptr))
	{
		if ((local_current_t <= local_death_t))
		{
			printf("%lld %lld %s\n", cur_time_millisec(philo),
				philo->index, status);
			pthread_mutex_unlock(local_mutex_ptr);
			philo->current_t = get_microseconds_sync();
			return (true);
		}
	}
	do_exit(philo, true);
	return (false);
}

// not performance critical (only called in death loop)
bool	check_exit(t_philo *restrict const philo)
{
	bool			*local_ptr;
	pthread_mutex_t	*local_mutex_ptr;

	local_ptr = philo->exit;
	local_mutex_ptr = philo->status;
	philo->current_t = get_microseconds();
	pthread_mutex_lock(local_mutex_ptr);
	if (!(*local_ptr))
	{
		pthread_mutex_unlock(local_mutex_ptr);
		if (philo->current_t <= philo->death_t)
		{
			return (false);
		}
		do_exit(philo, false);
		return (true);
	}
	do_exit(philo, true);
	return (true);
}
