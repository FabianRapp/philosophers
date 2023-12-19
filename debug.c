/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:14:06 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 18:57:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print_philo(t_philo *philo, bool timings, bool check_mutexes)
{
	printf("--------------\n");
	printf("philo %d:\n", philo->index);
	if (timings)
	{
		printf("\tstarve_ti: %d\n\teat_ti: %d\n\tsleep_ti: %d\n", philo->starve_ti, philo->eat_ti, philo->sleep_ti);
		printf("---\n\teat_count: %d\n\ttotal_start_t %lld\n", philo->eat_count, philo->total_start_t);
	}
	printf("\tdeath_time: %lld\n", philo->death_time);
	if (check_mutexes)
	{
		printf("---\n");
		pthread_mutex_lock(&philo->left_fork->mutex_used);
		(philo->left_fork->used) ? printf("\tleft_fork->used: true\n") : printf("\tleft_fork->used: false\n");
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
		pthread_mutex_lock(&philo->right_fork->mutex_used);
		(philo->right_fork->used) ? printf("\tright_fork->used: true\n") : printf("\tright_fork->used: false\n");
		pthread_mutex_unlock(&philo->right_fork->mutex_used);
		pthread_mutex_lock(philo->mutex_exit);
		(*philo->exit) ? printf("\texit: true\n") : printf("\texit: false\n");
		pthread_mutex_unlock(philo->mutex_exit);
		printf("--------------\n");
	}
}

