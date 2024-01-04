/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/04 21:46:56 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
TODO:
	- handle philos death while waiting for forks
	- handle philos death in my_sleep_until
	- handle 1 philo only
	- handle early error returns and cleanup
Tests:
	- 1 800 200 200		does not work, litterly does nothing?
	- 5 800 200 200		works
	- 5 800 200 200 7	""
	- 4 410 200 200		works (does not die)
	- 4 310 200 100		works (should die)
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
	my_sleep_init(philo->current_t); // cant die before start
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);
	while (philo->eat_count)
	{
		if (!print_status(philo, "is thinking"))
			break ;
		if (!eat(philo))
			break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
		if (!print_status(philo, "is sleeping"))
			break ;
		if (!my_sleep_until_small(philo->current_t + philo->sleep_dur, philo))
			break ;
		philo->current_t = get_microseconds_main();
	}
	return (arg);
}

void	wait_threads(t_general *const general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		//printf("thread %d joined\n", i);
		i++;
	}

}

int	main(int ac, char *av[])
{
	t_general	general_unalined[2];
	t_general	*general;

	general = general_unalined;
	align_ptr((int8_t **)(&general));
	if (!input(ac, av, general))
		return (0);
	if (!init_philos(general))
		return (0);
	if (!intit_threading(general))
		return (0);
	wait_threads(general);
	cleanup(general);
	return (0);
}
