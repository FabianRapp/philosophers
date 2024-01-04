/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:14:06 by frapp             #+#    #+#             */
/*   Updated: 2024/01/04 21:46:44 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print_philo(t_philo *restrict const philo, bool timings, bool check_mutexes)
{
	printf("--------------\n");
	printf("philo %u:\n", philo->index);
	if (timings)
	{
		printf("\tstarve_ti: " DATA_TYPE_SPECIFIER "\n\teat_ti: " DATA_TYPE_SPECIFIER "\n\tsleep_ti: " DATA_TYPE_SPECIFIER "\n", philo->starve_dur / MICROSEC_TO_MILLISEC, philo->eat_dur / MICROSEC_TO_MILLISEC, philo->sleep_dur / MICROSEC_TO_MILLISEC);
		printf("---\n\teat_count: " DATA_TYPE_SPECIFIER "\n\tstart_t %lld\n", philo->eat_count, (long long)philo->start_t / MICROSEC_TO_MILLISEC);
	}
	printf("\tdeath_time: %lld\n", (long long)(philo->death_t / MICROSEC_TO_MILLISEC));
	if (check_mutexes)
	{
		printf("---\n");
		pthread_mutex_lock(&philo->left_fork->mutex_used);
		(philo->left_fork->used) ? printf("\tleft_fork->used: true\n") : printf("\tleft_fork->used: false\n");
		pthread_mutex_unlock(&philo->left_fork->mutex_used);
		pthread_mutex_lock(&philo->right_fork->mutex_used);
		(philo->right_fork->used) ? printf("\tright_fork->used: true\n") : printf("\tright_fork->used: false\n");
		pthread_mutex_unlock(&philo->right_fork->mutex_used);
		pthread_mutex_lock(philo->status);
		(*philo->exit) ? printf("\texit: true\n") : printf("\texit: false\n");
		pthread_mutex_unlock(philo->status);
		printf("--------------\n");
	}
}

