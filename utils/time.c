/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/03 17:25:38 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t	get_microseconds_time(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

bool	my_sleep_until_small(int64_t target_t, t_philo *philo)
{
	int32_t	first_sleep;
	int64_t	first_target;

	first_sleep = target_t - get_microseconds_time();
	if (first_sleep <= 0)
		return (true);
	first_sleep = (first_sleep >> 4);
	first_target = target_t - first_sleep - 1000;
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	if (check_exit(philo))
		return (false);
	first_sleep = first_sleep >> 1;
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	if (check_exit(philo))
		return (false);
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	if (check_exit(philo))
		return (false);
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	if (check_exit(philo))
		return (false);
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	if (check_exit(philo))
		return (false);
	if (get_microseconds_time() < first_target)
		usleep(first_sleep);
	while (get_microseconds_time() < target_t)
	{
		if (check_exit(philo))
			return (false);
		usleep(1000);
	}
	if (check_exit(philo))
		return (false);
	return (true);
}

bool	my_sleep_until_large(int64_t target_t, t_philo *philo)
{
	int64_t	first_target;
	int64_t	first_sleep;

	first_sleep = (target_t - get_microseconds_time());
	first_sleep >>= 1;
	first_target = target_t - first_sleep - 1000;
	if (get_microseconds_time() < first_target)
	{
		usleep(first_sleep);
		first_sleep >>= 1;
		if (check_exit(philo))
			return (false);
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
		first_sleep >>= 1;
		if (check_exit(philo))
			return (false);
		if (get_microseconds_time() < first_target)
			usleep(first_sleep);
	}
	while (get_microseconds_time() < target_t)
	{
		if (check_exit(philo))
			return (false);
		usleep(1000);
	}
	if (check_exit(philo))
		return (false);
	return (true);
}
