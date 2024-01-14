/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 00:10:11 by frapp             #+#    #+#             */
/*   Updated: 2024/01/14 02:32:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// is used when it is impossible that the philo survies with the given timings
// this allows optimisations in other cases
// performence critical
void	*main_loop_death(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	while (philo->eat_count-- != 0 && pickup_forks(philo))
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return (NULL);
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, SLEEP_MSG))
			return (NULL);
		if (!my_sleep_slow(philo->current_t + philo->sleep_dur, philo))
			return (NULL);
		if (!change_status(philo, THINKING_MSG))
			return (NULL);
	}
	return (NULL);
}

//TODO:
// somehow this loop makes philos die now mb?

// runs when there is extra time where the threads can go into sleep
//	so the other threads get compute (allows a huge philo count)
// performence critical
//my_sleep_accurate(philo->current_t + philo->sleep_dur);
//my_sleep_fast(philo->current_t + philo->sleep_dur);
void	main_loop_loose(t_philo *philo)
{
	philo->thinking_dur -= THINK_TIME_BUFFER;
	while (philo->eat_count-- && pickup_forks(philo))
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return ;
		}
		philo->next_eat_t = (philo->current_t + philo->eat_dur
				+ philo->sleep_dur + philo->thinking_dur);
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, SLEEP_MSG))
			return ;
		usleep(philo->sleep_dur);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, THINKING_MSG))
			return ;
		my_sleep_think(philo->next_eat_t);
	}
}

// runs when the estimated computed time is larger than the starve time
// performence critical
void	main_loop_critical_timings(t_philo *philo)
{
	while (philo->eat_count != 0)
	{
		if (!pickup_forks(philo))
			return ;
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, SLEEP_MSG))
			return ;
		my_sleep_accurate(philo->current_t + philo->sleep_dur);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->current_t));
		if (!change_status(philo, THINKING_MSG))
			return ;
		if (philo->eat_count > 0)
			philo->eat_count--;
	}
}

// not performence critical
void	*choose_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!intit_thread(philo))
		return (arg);
	if (philo->death_loop)
	{
		if (philo->debug >= DEBUG_VERBOSE)
			printf("death loop\n");
		main_loop_death(philo);
	}
	else if (philo->thinking_dur < THINK_TIME_BUFFER)
	{
		if (philo->debug >= DEBUG_VERBOSE)
			printf("acc loop\n");
		main_loop_critical_timings(philo);
	}
	else
	{
		if (philo->debug >= DEBUG_VERBOSE)
			printf("loose loop\n");
		main_loop_loose(philo);
	}
	return (arg);
}
