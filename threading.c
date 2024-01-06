/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:37:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/06 11:34:52 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

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

bool	intit_threading(t_general *const general)
{
	int				i;
	t_philo			*philo;
	pthread_t		*thread;


	if (general->even)
	{
		if (!general->death_loop)
			printf("even normal\n");
		else
			printf("even death loop\n");
	}
	else
	{
		if (!general->death_loop)
			printf("odd normal\n");
		else
			printf("odd death loop\n");
	}
	if (!general->philos->speed_mode)
	{
		printf("accuracy mode\n");
	}
	else
	{
		printf("speed mode\n");
	}
	general->start_t = get_microseconds();
	general->start_t += general->count * THREADING_INIT_TIME_MICRO;
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i;
		thread = general->threads + i;
		philo->start_t = general->start_t;
		if (general->even)
		{
			if (!general->death_loop)
			{
				if (pthread_create(thread, NULL, main_loop_even, philo))
					return (false);
				usleep(1);
			}
			else
			{
				if (pthread_create(thread, NULL, main_loop_even_death, philo))
					return (false);
				usleep(1);
			}
		}
		else
		{
			if (!general->death_loop)
			{
				// if (pthread_create(thread, NULL, main_loop_odd, philo))
				// 	return (false);
				// usleep(1);
				if (pthread_create(thread, NULL, main_loop_even, philo))
					return (false);
				usleep(1);
			}
			else
			{
				if (pthread_create(thread, NULL, main_loop_odd_death, philo))
					return (false);
				usleep(1);
			}
		}
		i++;
	}
	wait_threads(general);
	return (false);
}
