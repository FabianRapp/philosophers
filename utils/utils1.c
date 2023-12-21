/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 07:11:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>



void	my_sleep_until(long long target_time, t_philo *philo, char *str)
{
	while(my_gettime() < target_time - USLEEP_TIME)
	{
		if (check_exit(philo, str))
			return ;
		usleep(USLEEP_TIME);
	}
	while(my_gettime() < target_time)
	{
		if (check_exit(philo, str))
			return ;
	}
}

bool	eat(t_philo *philo)
{
	my_sleep_until(philo->next_eat, philo, "waiting eat sleep");
	if (philo->even)
	{
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
	}
	else
	{
		if (!pickup_left_fork(philo))
			return (0);
		if (!pickup_right_fork(philo))
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			return (0);
		}
	}
	printf("time left philo %d: %lld\n", philo->index, (long long)philo->death_time - (long long)philo->current_time);
	if (check_exit(philo, "pickup right fork"))
	 	return (false);
	printf("%lld %d is eating\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo, "end of eating"))
		return (0);
	philo->death_time = philo->current_time + philo->starve_ti;
	my_sleep_until(philo->current_time + philo->eat_ti, philo, "eat sleep");
	if (!drop_forks(philo))
		return (0);
	philo->current_time += philo->eat_ti;
	// if (philo->even)
	// 	philo->next_eat = philo->current_time + (philo->eat_ti >> 1);
	if (philo->even)
		philo->next_eat = USLEEP_TIME * 2 + 2;
	else
		philo->next_eat = NEXT_EAT;
	return (1);
}

bool	check_exit(t_philo *philo, char *str)
{
	long long	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{
		if (!(local_current_time > philo->death_time))
		{
			pthread_mutex_unlock(philo->mutex_exit);
			return (false);
		}
		else
		{
			printf("%lld %d died in %s\n", philo->current_time - philo->total_start_t, philo->index, str);
			if (philo->even)
			{
				printf("even (next eat time - total start: %lld\n", philo->next_eat - philo->total_start_t);
			}
			else
			{
				printf("not even; %lld before next eat\n", philo->next_eat - my_gettime());
			}
			*(philo->exit) = true;
			pthread_mutex_unlock(philo->mutex_exit);
			return (true);
		}
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (true);
}
