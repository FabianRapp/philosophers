/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 17:06:48 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

bool	eat(t_philo *philo)
{
	philo->current_t = philo->next_eat_t;
	if (!my_sleep_until_small(philo->next_eat_t, philo))
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
	philo->death_t = philo->current_t + philo->starve_dur;
	philo->next_eat_t = philo->current_t + philo->eat_wait_dur;
	if (!my_sleep_until_small(philo->current_t + philo->eat_dur, philo))
	{
		drop_forks(philo);
		return (false);
	}
	drop_forks(philo);
	philo->current_t += philo->eat_dur;
	return (1);
}
