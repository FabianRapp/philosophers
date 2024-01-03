/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 10:31:39 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

	// while (my_gettime() < target_time - 1)
	// {
	// 	if (check_exit(philo))
	// 		return (false);
	// 	usleep(1000);
	// }
	// while (my_gettime() < target_time)
	// {
	// 	//__asm__ volatile("nop");
	// }
//time = (int64_t)s_time.tv_sec << 10;
//time += s_time.tv_usec / 1000LL;



bool	eat(t_philo *philo)
{
	philo->current_time = philo->next_eat;
	if (!my_sleep_until(philo->next_eat, philo))
		return (false);
	if (!pickup_left_fork(philo))
		return (false);
	if (!pickup_right_fork(philo))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (false);
	}
	if (!print_status(philo, "is eating"))
	{
		drop_forks(philo);
		return (false);
	}
	philo->death_time = philo->current_time + philo->starve_ti;
	philo->next_eat = philo->current_time + philo->eat_wait_time;
	if (!my_sleep_until(philo->current_time + philo->eat_ti, philo))
	{
		drop_forks(philo);
		return (false);
	}
	drop_forks(philo);
	philo->current_time += philo->eat_ti;
	return (1);
}
