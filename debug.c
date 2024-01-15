/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 06:45:38 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 06:45:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	debug_info_loop_conditoning(t_general *general)
{
	if (general->death_loop || general->count == 1)
		printf("death loop\n");
	else if (general->philos->thinking_dur > THINK_TIME_BUFFER)
	{
		printf("loose loop\n");
	}
	else
	{
		printf("critical_timings loop \n");
	}
	printf("min exection time: %lld\n",
		calculate_iteration_time(general->count));
	printf("thinking_dur: %lld\n", general->philos->thinking_dur);
	usleep(2000000);
}

void	print_mutex_status(const char *name, bool status)
{
	if (status)
		printf("\t%s: true\n", name);
	else
		printf("\t%s: false\n", name);
}

void	print_timing_info(t_philo *philo)
{
	printf("\tcur time:%lld\n", get_microseconds() / 1000);
	printf("\tstarve_ti: %lld\n\teat_ti: %lld\n\tsleep_ti: %lld\n",
		(int64_t)(philo->starve_dur * MICROSEC_TO_MILLISEC_FACTOR),
		(int64_t)(philo->eat_dur * MICROSEC_TO_MILLISEC_FACTOR),
		(int64_t)(philo->sleep_dur * MICROSEC_TO_MILLISEC_FACTOR));
	printf("\teat_count: %lld\n\tstart_t %lld\n", philo->eat_count,
		(long long)(philo->start_t * MICROSEC_TO_MILLISEC_FACTOR));
}

void	print_basic_info(t_philo *philo)
{
	printf("philo %lld:\n", philo->index);
	printf("\tdeath_time: %lld\n", (long long)
		(philo->death_t * MICROSEC_TO_MILLISEC_FACTOR));
}

void	print_mutexes_info(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex_used);
	print_mutex_status("left_fork->used", philo->left_fork->used);
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_lock(&philo->right_fork->mutex_used);
	print_mutex_status("right_fork->used", philo->right_fork->used);
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_lock(philo->buffer_mutex);
	print_mutex_status("exit", *philo->exit);
	pthread_mutex_unlock(philo->buffer_mutex);
}

void	print_philo_info(
	t_philo *restrict const philo, bool timings, bool check_mutexes)
{
	if (!philo)
	{
		printf("Error: Null philosopher pointer\n");
		return ;
	}
	printf("--------------\n");
	print_basic_info(philo);
	if (timings)
	{
		print_timing_info(philo);
		printf("---\n");
	}
	if (check_mutexes)
	{
		print_mutexes_info(philo);
		printf("--------------\n");
	}
}
