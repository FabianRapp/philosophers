/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:37:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/08 16:44:48 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
void	wait_threads(t_general *const general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		i++;
	}
}

static void	debug_info(t_general *general)
{
	if (!general->death_loop)
		printf("normal loop\n");
	else
		printf("death loop\n");
	if (!(general->philos->thinking_dur > 60))
	{
		printf("accuracy mode\n");
	}
	else
	{
		printf("speed mode\n");
	}
}

// not performance critical
bool	intit_threading(t_general *const general)
{
	int				i;
	t_philo			*philo;
	pthread_t		*thread;

	debug_info(general);
	general->start_t = get_microseconds();
	general->start_t += general->count * MICROSEC_PER_THREAD_INIT_TIME;
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i;
		thread = general->threads + i;
		philo->start_t = general->start_t;
		if (!general->death_loop)
		{
			if (philo->thinking_dur > SPEED_BOUNDRY)
			{
				if (pthread_create(thread, NULL, main_loop_fast, philo))
					return (false);
			}
			else
			{
				if (pthread_create(thread, NULL, main_loop_accurate, philo))
					return (false);
			}
		}
		else
		{
			if (pthread_create(thread, NULL, main_loop_death, philo))
				return (false);
		}
		i++;
	}
	wait_threads(general);
	return (false);
}
