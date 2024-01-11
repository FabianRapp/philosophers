/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:37:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/12 00:37:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
void	wait_threads(t_general *const general, pthread_t *status_thread)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		i++;
	}
	usleep(2000);
	pthread_join(*status_thread, NULL);
}



// not performance critical
bool	intit_threading(t_general *const general)
{
	int				i;
	t_philo			*philo;
	pthread_t		*cur_thread;
	pthread_t		status_thread;

	debug_info_loop_conditoning(general);
	if (pthread_create(&status_thread, NULL, flush_output_loop, general))
		return (false);
	general->start_t = get_microseconds();
	general->start_t += general->count * MICROSEC_PER_THREAD_INIT_TIME;
	if (general->count == 1)
	{
		printf("only 1\n");
		philo = general->philos;
		philo->next_eat_t = 0;
		cur_thread = general->threads;
		philo->start_t = general->start_t;
		philo->thinking_dur = -10000;
		philo->next_eat_t = 0;
		philo->death_loop = true;
		intit_thread(philo);
		if (pthread_create(cur_thread, NULL, main_loop_death, philo))
			return (false);
	}
	else
	{
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
	}
	wait_threads(general, &status_thread);
	return (false);
}
