/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/03 10:48:29 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int64_t	get_microseconds(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

static inline int64_t	get_microseconds_static(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

int64_t	my_gettime(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000;
	time += s_time.tv_usec >> 10;
	return (time);
}

int64_t	get_millisecond_fast(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000;
	time += s_time.tv_usec >> 10;
	return (time);
}

int64_t	get_millisecond_precise(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000;
	time += s_time.tv_usec / 10;
	return (time);
}

void	my_sleep(int milliseconds)
{
	int64_t	target_time = get_millisecond_fast() + milliseconds - 2;
	while (get_millisecond_fast() < target_time) {
		usleep(1000);
	}
	target_time += 2;
	while (get_millisecond_precise() < target_time) {
		usleep(1000);
	}
}

bool	my_sleep_until(int64_t target_time, t_philo *philo)
{
	target_time *= 1000;
	while (get_microseconds_static() < target_time)
	{
		if (check_exit(philo))
			return (false);
		usleep(1000);
	}
	if (check_exit(philo))
		return (false);
	return (true);
}


// bool	my_sleep_until(int64_t target_time, t_philo *philo)
// {
// 	//target_time1 *= 1000;
// 	while (my_gettime() < target_time)
// 	{
// 		if (check_exit(philo))
// 			return (false);
// 		usleep(5000);
// 	}
// 	if (check_exit(philo))
// 		return (false);
// 	return (true);
// }
