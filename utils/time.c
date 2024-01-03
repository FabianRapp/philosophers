/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/03 23:31:51 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t	get_microseconds_time(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

bool	my_sleep_until_small(int64_t target_t, t_philo *philo)
{
	int32_t			first_sleep;
	int64_t			first_target;

	first_sleep = target_t - get_microseconds_time();
	if (first_sleep <= 0)
		return (true);
	first_sleep = (first_sleep >> 4);
	first_target = target_t - (first_sleep * 15) - 1000;
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);

	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
	if (check_exit(philo))
		return (false);
	while (get_microseconds_time() < target_t)
	{
		__asm__ volatile("PREFETCHNTA %0" : : "m" (philo->death_t));
		if (check_exit(philo))
			return (false);
		usleep(1000);
	}
	if (check_exit(philo))
		return (false);
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
// 	if (check_exit(philo))
// 		return (false);
// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);

// 	if (get_microseconds_time() < first_target)
// 		usleep(first_sleep);
// 	__builtin_prefetch(&philo->death_t, 0, 3);
// 	if (check_exit(philo))
// 		return (false);
// 	while (get_microseconds_time() < target_t)
// 	{
// 		__builtin_prefetch(&philo->death_t, 0, 3);
// 		if (check_exit(philo))
// 			return (false);
// 		usleep(1000);
// 	}
// 	if (check_exit(philo))
// 		return (false);
// 	return (true);
// }
