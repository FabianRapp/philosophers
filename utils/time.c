/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/04 21:07:19 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t __attribute__((always_inline))	get_microseconds_time(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

static inline bool __attribute__((always_inline))	check_exit_inline(t_philo *const philo)
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

bool	my_sleep_eating(const int64_t target_t)
{
	int32_t			first_sleep;
	int64_t			first_target;

	first_sleep = target_t - get_microseconds_time();
	if (first_sleep <= 0)
		return (true);
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
	}
	first_sleep = target_t - get_microseconds_time();
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
	}
	while (get_microseconds_time() < target_t)
	{
		usleep(1000);
	}
	return (true);
}

bool	my_sleep_until_small(const int64_t target_t, t_philo *const philo)
{
	int32_t			first_sleep;
	int64_t			first_target;

	first_sleep = target_t - get_microseconds_time();
	if (first_sleep <= 0)
		return (true);
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
		__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
		if (check_exit_inline(philo))
			return (false);
	}
	first_sleep = target_t - get_microseconds_time();
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
		__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
		if (check_exit_inline(philo))
			return (false);
	}
	while (get_microseconds_time() < target_t)
	{
		__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
		if (check_exit_inline(philo))
			return (false);
		usleep(1000);
	}
	if (check_exit_inline(philo))
		return (false);
	return (true);
}


bool	my_sleep_init(const int64_t target_t)
{
	int32_t			first_sleep;
	int64_t			first_target;

	first_sleep = target_t - get_microseconds_time();
	if (first_sleep <= 0)
		return (true);
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
	}
	first_sleep = target_t - get_microseconds_time();
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	for (int i = 0; i < 16; i++)
	{
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
	}
	while (get_microseconds_time() < target_t)
	{
		usleep(1000);
	}
	return (true);
}


// bool	my_sleep_until_small(int64_t target_t, t_philo *philo)
// {
// 	int32_t			first_sleep;
// 	int64_t			first_target;

// 	first_sleep = target_t - get_microseconds_time();
// 	if (first_sleep <= 0)
// 		return (true);
// 	first_sleep = (first_sleep >> 4);
// 	first_target = target_t - (first_sleep * 15) - 1000;
// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);
// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit_inline(philo))
// 		return (false);
// 	while (get_microseconds_time() < target_t)
// 	{
// 		__builtin_prefetch(&philo->death_t, 0, 3);
// 		if (check_exit_inline(philo))
// 			return (false);
// 		usleep(1000);
// 	}
// 	if (check_exit_inline(philo))
// 		return (false);
// 	return (true);
// }
