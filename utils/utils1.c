/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/05 21:58:16 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t	get_microseconds_eat(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

bool	pickup_forks_odd(t_philo *restrict const philo)
{
	if (!(philo->index % 2))
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
			return (false);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
		{
			drop_left_fork(philo);
			return (false);
		}
	}
	else
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
			return (false);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
		{
			drop_right_fork(philo);
			return (false);
		}
	}
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	return (true);
}

bool	init_eat_odd(t_philo *restrict const philo)
{
	if (!change_status(philo, EATING_MSG))
	{
		drop_forks(philo);
		return (false);
	}
	philo->death_t = philo->current_t + philo->starve_dur;
	philo->next_eat_t = philo->current_t + philo->eat_wait_dur;
	return (true);
}
bool	pickup_forks_even(t_philo *restrict const philo)
{
	if (!(philo->index % 2))
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
			return (false);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
		{
			drop_left_fork(philo);
			return (false);
		}
	}
	else
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
			return (false);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
		{
			drop_right_fork(philo);
			return (false);
		}
	}
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	return (true);
}
// bool	init_eat_even(t_philo *restrict const philo)
// {


// 	//philo->next_eat_t = philo->current_t + philo->eat_wait_dur;
// 	return (true);
// }

void	align_ptr(int8_t **ptr)
{
	int		size_past_last_line;

	size_past_last_line = ((uintptr_t)((*ptr)) % CACHE_LINE_SIZE);
	if (!size_past_last_line)
		return ;
	*ptr = *ptr + CACHE_LINE_SIZE - size_past_last_line;
}
