/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/12 00:44:30 by frapp            ###   ########.fr       */
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

// is used when it is impossible that the philo survies with the given timings
// this allows optimisations in other cases
// performence critical
void	*main_loop_death(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	while (1)
	{
		if (philo->eat_count == 0)
			return (NULL);
		if (!pickup_forks(philo))
			return (NULL);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return (NULL);
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, SLEEP_MSG))
			return (NULL);
		if (!my_sleep_slow(philo->current_t + philo->sleep_dur, philo))
			return (NULL);
		if (!change_status(philo, THINKING_MSG))
			return (NULL);
		if (philo->eat_count > 0)
			philo->eat_count--;
	}
}

// runs when there is extra time where the threads can go into sleep
//	so the other threads get compute (allows a huge philo count)
// performence critical
void	main_loop_loose(t_philo *philo)
{
	philo->thinking_dur -= THINK_TIME_BUFFER;
	while (1)
	{
		if (philo->eat_count == 0)
			return ;
		if (!pickup_forks(philo))
			return ;
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return ;
		}
		philo->next_eat_t = (philo->current_t + philo->eat_dur + philo->sleep_dur + philo->thinking_dur);
		philo->death_t = philo->current_t  + philo->starve_dur;
		philo->next_eat_t = (philo->current_t + philo->eat_dur + philo->sleep_dur + philo->thinking_dur);
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, SLEEP_MSG))
			return ;
		my_sleep_fast(philo->current_t + philo->sleep_dur);
		//my_sleep_accurate(philo->current_t + philo->sleep_dur);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, THINKING_MSG))
			return ;
		my_sleep_think(philo->next_eat_t);
		if (philo->eat_count > 0)
			philo->eat_count--;
	}
}

// runs when the estimated computed time is larger than the starve time
// performence critical
void	main_loop_critical_timings(t_philo *philo)
{
	while (1)
	{
		if (philo->eat_count == 0)
			return ;
		if (!pickup_forks(philo))
			return ;
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			return ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, SLEEP_MSG))
			return ;
		my_sleep_accurate(philo->current_t + philo->sleep_dur);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
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
		//printf("death loop\n");
		main_loop_death(philo);
	}
	else if (philo->thinking_dur < THINK_TIME_BUFFER)
	{
		//printf("acc loop\n");
		main_loop_critical_timings(philo);
	}
	else
	{
		//printf("loose loop\n");
		main_loop_loose(philo);
	}
	return (arg);
}

// not performance critical
int	a(int ac, char *av[])
{
	t_general	general_unalined[3];
	t_general	*general;

	general = general_unalined;
	align_ptr((int8_t **)(&general));
	if (!input(ac, av, general))
		return (0);
	if (!init_philos(general))
		return (0);
	if (!intit_threading(general))
		return (0);
	cleanup(general);
	return (0);
}

#include <stdlib.h>
int	main(int ac, char *av[])
{
	a(ac, av);
	//system("leaks p");
	return (0);
}