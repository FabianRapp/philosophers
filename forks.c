/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 05:15:23 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 11:30:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


bool	pickup_left_fork(t_philo *philo)
{
if (check_exit(philo, "pick up left fork"))
	return (false);

	pthread_mutex_lock(&philo->left_fork->mutex_used);
	while(philo->left_fork->used)
	{
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
		usleep(10);
		if (check_exit(philo, "pickup left fork1"))
			return (false);
		pthread_mutex_lock(&philo->left_fork->mutex_used);
	}
	philo->left_fork->used = true;
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the left fork\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo, "pick up left fork2"))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (false);
	}
	return (true);
}

bool	pickup_right_fork(t_philo *philo)
{
	if (check_exit(philo, "pickup right fork1"))
		return (false);

	pthread_mutex_lock(&philo->right_fork->mutex_used);
	while (philo->right_fork->used)
	{
		pthread_mutex_unlock(&philo->right_fork->mutex_used);
		if (check_exit(philo, "pickup right fork2"))
			return (false);
		pthread_mutex_lock(&philo->right_fork->mutex_used);
	}
	philo->right_fork->used = true;
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the right fork\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo, "pickup right fork2"))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (false);
	}
	return (true);
}

void	drop_right_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->mutex_used);
	philo->right_fork->used = false;
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	drop_left_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex_used);
	philo->left_fork->used = false;
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

bool drop_forks(t_philo *philo)
{
	if (philo->index % 2)
	{
		drop_right_fork(philo);
		drop_left_fork(philo);
	}
	else
	{
		drop_left_fork(philo);
		drop_right_fork(philo);
	}
	if (check_exit(philo, "drop forks"))
		return (false);
	return (true);
}
