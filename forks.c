/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 05:15:23 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 12:26:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


static inline long long	my_gettime(void)
{
	struct timeval s_time;
	long long time;

	gettimeofday(&s_time, NULL);
	time = (long long)s_time.tv_sec * 1000LL;
	time += (long long)s_time.tv_usec >> 10;
	//time += (long long)s_time.tv_usec / 1000LL;
	return (time);
}

bool	pickup_left_fork(t_philo *philo)
{
if (check_exit(philo))
	return (false);

	pthread_mutex_lock(&philo->left_fork->mutex_used);
	while(philo->left_fork->used)
	{
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
		usleep(USLEEP_TIME_MINI);
		if (check_exit(philo))
			return (false);
		pthread_mutex_lock(&philo->left_fork->mutex_used);
	}
	philo->left_fork->used = true;
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the left fork\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (false);
	}
	return (true);
}

bool	pickup_right_fork(t_philo *philo)
{
	if (check_exit(philo))
		return (false);

	pthread_mutex_lock(&philo->right_fork->mutex_used);
	while (philo->right_fork->used)
	{
		pthread_mutex_unlock(&philo->right_fork->mutex_used);
		usleep(USLEEP_TIME_MINI);
		if (check_exit(philo))
			return (false);
		pthread_mutex_lock(&philo->right_fork->mutex_used);
	}
	philo->right_fork->used = true;
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the right fork\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo))
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
	if (check_exit(philo))
		return (false);
	return (true);
}
