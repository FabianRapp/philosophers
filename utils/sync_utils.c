/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 07:30:05 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 09:47:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// performence critical
static inline int64_t __attribute__((always_inline))
	get_microseconds_sync(void)
{
	struct timeval	s_time;
	int64_t			time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

// this function is responsible for the mutex unlock
// not performance critical
static void	do_exit(t_philo *restrict const philo)
{
	pthread_mutex_lock(&philo->testing_exit_mutext);
	if (!(philo->testing_exit))
	{
		pthread_mutex_unlock(&philo->testing_exit_mutext);
		pthread_mutex_lock(philo->buffer_mutex);
		*(philo->output_size) += put_output_to_buffer(get_microseconds_sync()
				- philo->start_t, philo->index, *(philo->output_buffer)
				+ *(philo->output_size), " died\n");
		pthread_mutex_unlock(philo->buffer_mutex);
	}
	else
		pthread_mutex_unlock(&philo->testing_exit_mutext);
}

// performance critical in forks and death loop
bool	is_dead(t_philo *restrict const philo)
{
	if (philo->death_t >= get_microseconds())
	{
		
		return (false);
	}
	do_exit(philo);
	return (true);
}

// performence critical
bool	change_status(t_philo *restrict const philo,
	const char *restrict status)
{
	int64_t			local_current_t;
	int64_t			local_death_t;
	pthread_mutex_t	*local_buffer_mutex;

	local_death_t = philo->death_t;
	local_buffer_mutex = philo->buffer_mutex;
	pthread_mutex_lock(&philo->testing_exit_mutext);
	if (!(philo->testing_exit))
	{
		pthread_mutex_unlock(&philo->testing_exit_mutext);
		pthread_mutex_lock(local_buffer_mutex);
		local_current_t = get_microseconds_sync();
		if (local_death_t >= local_current_t)
		{
			philo->current_t = local_current_t;
			*(philo->output_size) += put_output_to_buffer((local_current_t
						- philo->start_t), philo->index, *(philo->output_buffer)
					+ *(philo->output_size), status);
			pthread_mutex_unlock(local_buffer_mutex);
			return (true);
		}
		pthread_mutex_unlock(local_buffer_mutex);
	}
	else
		pthread_mutex_unlock(&philo->testing_exit_mutext);
	do_exit(philo);
	return (false);
}
