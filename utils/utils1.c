/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/04 23:08:43 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t	get_microseconds_eat(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

bool	eat(t_philo *restrict const philo)
{
	if (!my_sleep_until_small(philo->next_eat_t, philo))
		return (false);
	if (!pickup_left_fork(philo)) // left first should be better for cache
		return (false);
	if (!pickup_right_fork(philo))
	{
		drop_left_fork(philo);
		return (false);
	}
	if (!print_status(philo, "is eating"))
	{
		drop_forks(philo);
		return (false);
	}
	philo->death_t = philo->current_t + philo->starve_dur;
	philo->next_eat_t = philo->current_t + philo->eat_wait_dur;
	my_sleep_eating(philo->current_t + philo->eat_dur);
	if (drop_forks(philo))
	{
		philo->current_t = get_microseconds_eat();
		return (true);
	}
	return (false);
}

void	align_ptr(int8_t **ptr)
{
	int		size_past_last_line;

	size_past_last_line = ((uintptr_t)((*ptr)) % CACHE_LINE_SIZE);
	if (!size_past_last_line)
		return ;
	*ptr = *ptr + CACHE_LINE_SIZE - size_past_last_line;
}
