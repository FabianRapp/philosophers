/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:37:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/14 00:27:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// performence critical ending
bool	intit_thread(t_philo *restrict const philo)
{
	philo->current_t = philo->start_t;
	philo->next_eat_t += philo->current_t;
	philo->death_t = philo->start_t;
	philo->death_t += philo->starve_dur;
	if (philo->thinking_dur > THINK_TIME_BUFFER)
		my_sleep_fast(philo->current_t);
	else
		my_sleep_accurate(philo->current_t);
	if (!change_status(philo, THINKING_MSG))
		return (false);
	if (philo->death_loop)
	{
		if (!my_sleep_slow(philo->next_eat_t, philo))
			return (false);
	}
	else
	{
		my_sleep_think(philo->next_eat_t);
	}
	return (true);
}

// not performance critical
void	wait_threads(t_general *const general, pthread_t *status_thread)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		if (general->debug >= DEBUG_VERBOSE)
			printf("%d joined\n", i);
		pthread_join(general->threads[i], NULL);
		i++;
	}
	pthread_mutex_lock(&general->status_mutex);
	general->exit = true;
	pthread_mutex_unlock(&general->status_mutex);
	usleep(2000);
	if (general->debug >= DEBUG_INFO)
		printf("all philos joined\n");
	pthread_join(*status_thread, NULL);
}

// not performance critical
bool	intit_threading(t_general *const general)
{
	int				i;
	t_philo			*philo;
	pthread_t		*cur_thread;
	pthread_t		status_thread;

	if (general->debug >= DEBUG_INFO)
		debug_info_loop_conditoning(general);
	if (pthread_create(&status_thread, NULL, flush_output_loop, general))
		return (false);
	general->start_t = get_microseconds();
	general->start_t += general->count * MICROSEC_PER_THREAD_INIT_TIME;
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i;
		cur_thread = general->threads + i;
		philo->start_t = general->start_t;
		if (pthread_create(cur_thread, NULL, choose_loop, philo))
			return (false);
		i++;
	}
	if (general->debug >= DEBUG_VERBOSE)
		printf("all threads started\n");
	wait_threads(general, &status_thread);
	return (false);
}
