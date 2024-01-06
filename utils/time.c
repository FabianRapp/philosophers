/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/05 20:25:18 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int64_t	get_microseconds(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

static inline int64_t __attribute__((always_inline))
	get_microseconds_time(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

static inline bool __attribute__((always_inline))
	check_exit_time(t_philo *restrict const philo)
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
			return (false);
		}
		return (do_exit(philo, false));
	}
	return (do_exit(philo, true));
}

void	my_sleep_fast(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE_SPEED)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	usleep(HARDCODE_SLEEP_SPEED);
}

void	my_sleep_accurate(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE_ACC)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	while (target_t > get_microseconds_time())
	{
	}
	//usleep(HARDCODE_SLEEP);
}

bool	my_sleep_slow(const int64_t target_t,
	t_philo *restrict const philo)
{
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	if (check_exit_time(philo))
		return (false);
	while (get_microseconds_time() < target_t - SLEEP_TOLERANCE_SPEED)
	{
		usleep(SLEEP_TOLERANCE_SPEED);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (check_exit_time(philo))
			return (false);
	}
	// while (get_microseconds_time() < target_t)
	// {
	// 	usleep(1);
	// 	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	// 	if (check_exit_time(philo))
	// 		return (false);
	// }
	while (target_t > get_microseconds_time())
	{
	}
	return (!check_exit_time(philo));
}
