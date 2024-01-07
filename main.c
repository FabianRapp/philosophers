/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/06 12:39:21 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
6000 800 200 200 100	works

*/

static inline int64_t	get_microseconds_main(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

static inline void	intit_thread(t_philo *restrict const philo)
{
	philo->current_t = philo->start_t;
	philo->next_eat_t += philo->current_t;
	philo->death_t = philo->start_t;
	philo->death_t += philo->starve_dur;
	 int64_t	sleep_time = philo->current_t - get_microseconds_main();
	// if (philo->speed_mode)
	// 	my_sleep_fast(philo->current_t);
	if (philo->speed_mode)
	{
		if (sleep_time > 10)
		{
			usleep(sleep_time);
		}
	}
	else
		my_sleep_accurate(philo->current_t);
}

// is used when it is impossible that the philo survies with the given timings
// this allows optimisations in the regular cases
void	*main_loop_even_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);
	if (!change_status(philo, THINKING_MSG))
		return (arg);
	if (!my_sleep_slow(philo->next_eat_t, philo))
		return (false);
	while (philo->eat_count)
	{
		if (!pickup_forks_even(philo))
			break ;
		if (!change_status(philo, EATING_MSG))
		{
			drop_forks(philo);
			break ;
		}
		philo->death_t = philo->current_t + philo->starve_dur;
		my_sleep_slow(philo->current_t + philo->eat_dur, philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!drop_forks(philo))
			break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
		if (!change_status(philo, SLEEP_MSG))
			break ;
		if (!my_sleep_slow(philo->current_t + philo->sleep_dur, philo))
			break ;
		if (!change_status(philo, THINKING_MSG))
			break ;
	}
	return (arg);
}

/*
is used when it is impossible that the philo survies with
the given timings, this allows optimisations in the regular cases
*/
void	*main_loop_odd_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);

	if (!my_sleep_slow(philo->next_eat_t, philo))
		return (false);
	while (philo->eat_count)
	{
		if (!change_status(philo, THINKING_MSG))
			break ;
		if (!pickup_forks_odd(philo))
			break ;
		if (!init_eat_odd(philo))
			break ;
		my_sleep_slow(philo->current_t + philo->eat_dur, philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
			if (!drop_forks(philo))
				break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
		if (!change_status(philo, SLEEP_MSG))
			break ;
		if (!my_sleep_slow(philo->current_t + philo->sleep_dur, philo))
			break ;
	}
	return (arg);
}

void	*main_loop_even(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);
	if (!change_status(philo, THINKING_MSG))
		return (arg);
	if (philo->speed_mode)
	{
		int64_t	sleep_time = philo->next_eat_t - get_microseconds_main();
		if (sleep_time > 10)
			usleep(sleep_time);
		//my_sleep_fast(philo->next_eat_t);
		//bool first = true;
		while (philo->eat_count)
		{
			// int64_t	sleep_time = philo->next_eat_t - get_microseconds_main();
			// if (sleep_time > 10)
			// {
			// 	usleep((int)sleep_time);
			// }
			//if (!first)
				usleep(philo->eat_wait_dur);
			//else
			//	first = false;
			if (!pickup_forks_even(philo))
				break ;
			if (!change_status(philo, EATING_MSG))
			{
				drop_forks(philo);
				break ;
			}
			philo->death_t = philo->current_t + philo->starve_dur;
			//philo->next_eat_t = philo->current_t + philo->eat_wait_dur;
		//	sleep_time = philo->current_t + philo->eat_dur - get_microseconds_main();
			//if (sleep_time > 10)
				usleep(philo->eat_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
			if (!drop_forks(philo))
				break ;
			if (philo->eat_count > 0)
				philo->eat_count--;
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
			if (!change_status(philo, SLEEP_MSG))
				break ;
			//sleep_time = philo->current_t + philo->sleep_dur - get_microseconds_main();
			//if (sleep_time > 10)
				usleep(philo->sleep_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
			if (!change_status(philo, THINKING_MSG))
				break ;

		}
	}
	else
	{
		my_sleep_accurate(philo->next_eat_t);
		while (philo->eat_count)
		{
			if (!pickup_forks_even(philo))
				break ;
			if (!change_status(philo, EATING_MSG))
			{
				drop_forks(philo);
				break ;
			}
			philo->death_t = philo->current_t + philo->starve_dur;
			my_sleep_accurate(philo->current_t + philo->eat_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
			if (!drop_forks(philo))
				break ;
			if (philo->eat_count > 0)
				philo->eat_count--;
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
			if (!change_status(philo, SLEEP_MSG))
				break ;
			my_sleep_accurate(philo->current_t + philo->sleep_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
			if (!change_status(philo, THINKING_MSG))
				break ;
			//philo->current_t = get_microseconds_main();
		}
	}
	return (arg);
}

void	*main_loop_odd(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);
	if (philo->speed_mode)
	{
		while (philo->eat_count)
		{
			if (!change_status(philo, THINKING_MSG))
				break ;
			int64_t	sleep_time = philo->next_eat_t - get_microseconds_main();
			if (sleep_time > 10)
				usleep((int)sleep_time);
			//my_sleep_fast(philo->next_eat_t);
			if (!pickup_forks_odd(philo))
				break ;
			if (!init_eat_odd(philo))
				break ;
			sleep_time = philo->current_t + philo->eat_dur - get_microseconds_main();
			if (sleep_time > 10)
				usleep((int)sleep_time);
			//my_sleep_fast(philo->current_t + philo->eat_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
			if (!drop_forks(philo))
				break ;
			if (philo->eat_count > 0)
				philo->eat_count--;
			if (!change_status(philo, SLEEP_MSG))
				break ;
			sleep_time = philo->current_t + philo->sleep_dur - get_microseconds_main();
			if (sleep_time > 10)
				usleep((int)sleep_time);
			//my_sleep_fast(philo->current_t + philo->sleep_dur);
			philo->current_t = get_microseconds_main();
		}
	}
	else
	{
		while (philo->eat_count)
		{
			if (!change_status(philo, THINKING_MSG))
				break ;
			my_sleep_accurate(philo->next_eat_t);
			if (!pickup_forks_odd(philo))
				break ;
			if (!init_eat_odd(philo))
				break ;
			my_sleep_accurate(philo->current_t + philo->eat_dur);
			__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
			if (!drop_forks(philo))
				break ;
			if (philo->eat_count > 0)
				philo->eat_count--;
			if (!change_status(philo, SLEEP_MSG))
				break ;
			my_sleep_accurate(philo->current_t + philo->sleep_dur);
			philo->current_t = get_microseconds_main();
		}
	}
	return (arg);
}

int	main(int ac, char *av[])
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

// 1901:

//50140
//50139
//50138

// 1903
// 50139

// 1901 800 200 200 100
// usleep:
//60873
// my_sleep_fast:
//60998
