/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/08 18:37:26 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
6000 800 200 200 100	works

*/

// static inline int64_t	get_microseconds_main(void)
// {
// 	struct timeval	s_time;

// 	gettimeofday(&s_time, NULL);
// 	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
// }


// performence critical ending
static inline bool	intit_thread(t_philo *restrict const philo)
{
	philo->current_t = philo->start_t;
	philo->next_eat_t += philo->current_t;
	philo->death_t = philo->start_t;
	philo->death_t += philo->starve_dur;
	if (philo->thinking_dur > SPEED_BOUNDRY)
		my_sleep_fast(philo->current_t);
	else
		my_sleep_accurate(philo->current_t);
	if (!change_status(philo, THINKING_MSG))
		return (false);
	return (true);
}

// is used when it is impossible that the philo survies with the given timings
// this allows optimisations in the regular cases
// performence critical
void	*main_loop_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!intit_thread(philo))
		return (arg);
	if (!my_sleep_slow(philo->next_eat_t, philo))
		return (false);
	while (philo->eat_count)
	{
		if (!pickup_forks(philo))
			break ;
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			break ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		drop_forks(philo);
		if (!change_status(philo, SLEEP_MSG))
			break ;
		if (!my_sleep_slow(philo->current_t + philo->sleep_dur, philo))
			break ;
		if (!change_status(philo, THINKING_MSG))
			break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
	}
	return (arg);
}

// performence critical
void	*main_loop_fast(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!intit_thread(philo))
		return (arg);
	my_sleep_fast(philo->next_eat_t);
	while (philo->eat_count)
	{
		if (!pickup_forks(philo))
			break ;
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			break ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		philo->next_eat_t = (philo->current_t + philo->eat_dur + philo->sleep_dur + philo->thinking_dur);
		my_sleep_fast(philo->current_t + philo->eat_dur);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		drop_forks(philo);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, SLEEP_MSG))
			break ;
		my_sleep_fast(philo->current_t + philo->sleep_dur);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, THINKING_MSG))
			break ;
		//my_sleep_fast(philo->next_eat_t);
		my_sleep_accurate(philo->next_eat_t);
		//usleep(philo->thinking_dur);
		if (philo->eat_count > 0)
			philo->eat_count--;
	}
	return (arg);
}

// performence critical
void	*main_loop_accurate(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!intit_thread(philo))
		return (arg);
	my_sleep_accurate(philo->next_eat_t);
	while (philo->eat_count)
	{
		if (!pickup_forks(philo))
			break ;
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			break ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_accurate(philo->current_t + philo->eat_dur);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		drop_forks(philo);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, SLEEP_MSG))
			break ;
		my_sleep_accurate(philo->current_t + philo->sleep_dur);
		//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (!change_status(philo, THINKING_MSG))
			break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
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