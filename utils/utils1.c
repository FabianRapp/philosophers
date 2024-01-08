/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/08 18:24:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// performence critical
bool	pickup_forks(t_philo *restrict const philo)
{
	if (!(philo->index % 2))
	{
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
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
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
		{
			drop_right_fork(philo);
			return (false);
		}
	}
	//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	return (true);
}
