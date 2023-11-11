/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 04:56:21 by frapp             #+#    #+#             */
/*   Updated: 2023/11/09 04:57:11 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// returns true for exit
bool	check_exit(t_philo *philo)
{
	pthread_mutex_lock(philo->exit_mutex);
	if (!*(philo->exit))
	{
		pthread_mutex_unlock(philo->exit_mutex);
		return (false);
	}
	pthread_mutex_unlock(philo->exit_mutex);
	return (true);
}
