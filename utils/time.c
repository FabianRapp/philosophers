/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:31:41 by fabi              #+#    #+#             */
/*   Updated: 2024/01/11 16:58:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
int64_t	get_microseconds(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * SEC_TO_MICROSEC_FACTOR;
	time += s_time.tv_usec;
	return (time);
}

// performence critical
static inline int64_t __attribute__((always_inline))
	get_microseconds_time(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * SEC_TO_MICROSEC_FACTOR + s_time.tv_usec);
}

// sleeps atleast until target_t
// performence critical; used in standard cases
// sleeps atleast until target_t
// for some reason this is much more accurate that
// just usleep(sleeptime)
void	my_sleep_fast(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE_FAST)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
	usleep(SLEEP_TOLERANCE_FAST);
}

// sleeps atleast until target_t
// does sleep atleast until target_t and max very few micro sec more
// performence critical
// used for eating to unlock forks as fast as possible and when the given timings might be too tight
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
}

// sleeps atleast until target_t
// can sleep more or less than the target
// performence critical
void	my_sleep_think(const int64_t target_t)
{
	int64_t			time_diff;
	int64_t			temp_sleep;

	time_diff = target_t - get_microseconds_time();
	temp_sleep = time_diff - (time_diff >> 3);
	while (temp_sleep > SLEEP_TOLERANCE_FAST)
	{
		usleep(temp_sleep);
		time_diff = target_t - get_microseconds_time();
		temp_sleep = time_diff - (time_diff >> 3);
	}
}

// sleeps atleast until target_t
// performence critical; used when it is impossible that the philo survies
bool	my_sleep_slow(const int64_t target_t,
	t_philo *restrict const philo)
{
	while (target_t > get_microseconds_time())
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (is_dead(philo))
			return (false);
	}
	return (!is_dead(philo));
}
