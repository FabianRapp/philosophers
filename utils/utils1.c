/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 12:33:28 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


static inline long long	my_gettime(void)
{
	struct timeval s_time;
	long long time;

	gettimeofday(&s_time, NULL);
	time = (long long)s_time.tv_sec * 1000LL;
	//time = (long long)s_time.tv_sec << 10;
	time += (long long)s_time.tv_usec >> 10;
	//time += (long long)s_time.tv_usec / 1000LL;
	return (time);
}

void	my_sleep_until(long long target_time, t_philo *philo)
{
	while(my_gettime() < target_time - USLEEP_TIME)
	{
		if (check_exit(philo))
			return ;
		usleep(USLEEP_TIME);
	}
	while(my_gettime() < target_time)
	{
		//__asm__ volatile("nop");
	}
	if (check_exit(philo))
		return ;
}

bool	eat(t_philo *philo)
{
	my_sleep_until(philo->next_eat, philo);
	if (philo->index % 2)
	{
		if (!pickup_left_fork(philo))
			return (0);
		if (!pickup_right_fork(philo))
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			return (0);
		}
	}
	else
	{
		if (!pickup_right_fork(philo))
			return (0);
		if (!pickup_left_fork(philo))
		{
			pthread_mutex_unlock(&philo->right_fork->mutex);
			return (0);
		}
	}
	if (check_exit(philo))
	 	return (false);
	printf("%llu %d is eating\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo))
		return (0);
	philo->death_time = philo->current_time + philo->starve_ti;
	my_sleep_until(philo->current_time + philo->eat_ti, philo);
	if (!drop_forks(philo))
		return (0);
	philo->current_time += philo->eat_ti;
	philo->next_eat = philo->current_time + philo->eat_wait_time;
	return (1);
}

void	kill_philo(t_philo *philo)
{
	printf("%lld %d died\n", philo->current_time - philo->total_start_t, philo->index);
	printf("even (next eat time - total start: %lld\n", philo->next_eat - philo->total_start_t);
	*(philo->exit) = true;
	pthread_mutex_unlock(philo->mutex_exit);
}

bool	check_exit(t_philo *philo)
{
	int64_t	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{
		pthread_mutex_unlock(philo->mutex_exit);
		if (!(local_current_time > philo->death_time))
		{
			return (false);
		}
		else
		{
			pthread_mutex_lock(philo->mutex_exit);
			kill_philo(philo);
			return (true);
		}
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (true);
}
