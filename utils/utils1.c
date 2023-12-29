/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/29 10:03:03 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


int64_t	my_gettime(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000;
	//time = (int64_t)s_time.tv_sec << 10;
	time += s_time.tv_usec >> 10;
	//time += s_time.tv_usec / 1000LL;
	return (time);
}

bool	my_sleep_until(int64_t target_time, t_philo *philo)
{
	int64_t		target_time1;
	//int64_t	sleep_time;

	target_time1 = target_time - 1;
	// sleep_time = (target_time1 - my_gettime());
	// if (sleep_time > 0)
	// 	usleep(sleep_time);
	//target_time1 -= sleep_time / 1000;
	while(my_gettime() < target_time1)
	{
		if (check_exit(philo))
			return (false);
		usleep(1);
	}
	while(my_gettime() < target_time)
	{
		//__asm__ volatile("nop");
	}
	if (check_exit(philo))
		return (false);
	return (true);
}

bool	eat(t_philo *philo)
{
	philo->current_time = philo->next_eat;
	if (!my_sleep_until(philo->next_eat, philo))
		return (false);
	// if (philo->index % 2)
	// {
		if (!pickup_left_fork(philo))
			return (0);
		if (!pickup_right_fork(philo))
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			return (0);
		}
	// }
	// else
	// {
	// 	if (!pickup_right_fork(philo))
	// 		return (0);
	// 	if (!pickup_left_fork(philo))
	// 	{
	// 		pthread_mutex_unlock(&philo->right_fork->mutex);
	// 		return (0);
	// 	}
	// }
	if (!print_status(philo, "is eating"))
		return (false);
	philo->death_time = philo->current_time + philo->starve_ti;
	philo->next_eat = philo->current_time + philo->eat_ti + philo->sleep_ti;
	// if (!my_sleep_for(philo->eat_ti - 1, philo))
	// 	return (false);
	if (!my_sleep_until(philo->current_time + philo->eat_ti, philo))
		return (false);
	drop_forks(philo);
	philo->current_time += philo->eat_ti;
	//philo->next_eat = philo->current_time + philo->eat_wait_time;
	return (1);
}
