/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 05:06:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
//#define DEBUG

void	my_sleep_until(long long target_time, t_philo *philo, char *str)
{
	while(my_gettime() < target_time)
	{
		if (check_exit(philo, str))
			return ;
	}
}

bool	pickup_left_fork(t_philo *philo)
{
	if (check_exit(philo, "pick up left fork"))
		return (false);

	#ifndef DEBUG
		pthread_mutex_lock(&philo->left_fork->mutex_used);
		while(philo->left_fork->used)
		{
			pthread_mutex_unlock(&philo->left_fork->mutex_used);
			if (check_exit(philo, "pickup left fork1"))
				return (false);
			pthread_mutex_lock(&philo->left_fork->mutex_used);
		}
		philo->left_fork->used = true;
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
	#endif
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the left fork\n", philo->current_time - philo->total_start_t, philo->index);
	//printf("%lld %d has taken the left fork\n", my_gettime() - philo->total_start_t, philo->index);
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

	#ifndef DEBUG
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
	#endif
	pthread_mutex_lock(&philo->right_fork->mutex);
	//printf("%lld %d has taken the right fork\n", my_gettime() - philo->total_start_t, philo->index);
	philo->current_time = my_gettime();
	printf("%lld %d has taken the right fork\n", philo->current_time - philo->total_start_t, philo->index);
	if (check_exit(philo, "pickup right fork"))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (false);
	}
	return (true);
}

bool	eat(t_philo *philo)
{
	static __thread int		eat_cout = 0; //debugging

	my_sleep_until(philo->next_eat, philo, "waiting eat sleep");
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
	printf("time left philo %d: %lld\n", philo->index, (long long)philo->death_time - (long long)philo->current_time);
	if (check_exit(philo, "pickup right fork"))
	 	return (false);
	printf("%lld %d is eating for the %d. time\n", philo->current_time - philo->total_start_t, philo->index, ++eat_cout);
	if (check_exit(philo, "end of eating"))
		return (0);
	philo->death_time = philo->current_time + philo->starve_ti;
	philo->next_eat = NEXT_EAT;//philo->death_time + philo->eat_ti / 2 + philo->sleep_ti / 2;
	my_sleep_until(philo->current_time + philo->eat_ti, philo, "eat sleep");
	if (!drop_forks(philo))
		return (0);
	philo->current_time += philo->eat_ti;
	return (1);
}

void	drop_right_fork(t_philo *philo)
{
	#ifndef DEBUG
		pthread_mutex_lock(&philo->right_fork->mutex_used);
		philo->right_fork->used = false;
		pthread_mutex_unlock(&philo->right_fork->mutex_used);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	#else
		pthread_mutex_unlock(&philo->right_fork->mutex);
	#endif
}

void	drop_left_fork(t_philo *philo)
{
	#ifndef DEBUG
		pthread_mutex_lock(&philo->left_fork->mutex_used);
		philo->left_fork->used = false;
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
		pthread_mutex_unlock(&philo->left_fork->mutex);
	#else
		pthread_mutex_unlock(&philo->left_fork->mutex);
	#endif
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

bool	check_exit(t_philo *philo, char *str)
{
	long long	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{
		if (!(local_current_time >= philo->death_time))
		{
			pthread_mutex_unlock(philo->mutex_exit);
		}
		else
		{
			printf("%lld %d died in %s; %lld before next eat\n", philo->current_time - philo->total_start_t, philo->index, str, philo->next_eat - my_gettime());
			*(philo->exit) = true;
			pthread_mutex_unlock(philo->mutex_exit);
			return (true);
		}
		return (false);
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (true);
}
