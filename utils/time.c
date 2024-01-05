/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/04 23:00:54 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t __attribute__((always_inline))	get_microseconds_time(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

static inline bool __attribute__((always_inline))	check_exit_inline(t_philo *restrict const philo)
{
	bool			*local_ptr;
	pthread_mutex_t	*local_mutex_ptr;

	local_ptr = philo->exit;
	local_mutex_ptr = philo->status;
	philo->current_t = get_microseconds_time();
	pthread_mutex_lock(local_mutex_ptr);
	if (!(*local_ptr))
	{
		pthread_mutex_unlock(local_mutex_ptr);
		if (philo->current_t <= philo->death_t)
		{
			return (false); // 99.99999999 the returns of this function
		}
		return (do_exit(philo, false));
	}
	return (do_exit(philo, true));
}

void	my_sleep_eating(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	usleep(HARDCODE_SLEEP);
}

bool	my_sleep_until_small(const int64_t target_t, t_philo *restrict const philo)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE)
	{
		usleep(temp_sleep);
		PREFETCH_EXIT;
		if (check_exit_inline(philo))
			return (false);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	usleep(HARDCODE_SLEEP);
	PREFETCH_EXIT;
	return (!check_exit_inline(philo));
}

void	my_sleep_init(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	usleep(HARDCODE_SLEEP);
}
