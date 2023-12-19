/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 19:19:33 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
TODO:
	- handle philos death while waiting for forks
	- handle 1 philo only
	- handle early error returns and cleanup

Tests:
	- 1 800 200 200			need better fork logic
	- 1 800 200 200			does not work (one dies)
	- 5 800 200 200 7		does not work (one dies)
	- 4 410 200 200			works (none should die)
	- 4 310 200 100			works (one should die)
*/

long long	action(t_philo *philo)
{
	long long	next_time;

	// if (check_exit(philo, "start action"))
	// 	return (0);
	printf("%lld %d is thinking\n", my_gettime() - philo->total_start_t, philo->index);
	philo->current_time = eat(philo);
	if (!philo->current_time)
		return (0);
	// printf("%lld %d is eating for the %d. time\n", my_gettime(philo->total_start_t), philo->index, ++eat_cout);
	if (philo->eat_count > 0)
		philo->eat_count--;
	if (check_exit(philo, "action"))
		return (0);
	printf("%lld %d is sleeping\n", philo->current_time - philo->total_start_t, philo->index);
	//if (!(philo->sleep_ti < MIN_SLEEP_T))
	next_time = philo->sleep_ti + philo->current_time;
		my_sleep_until(next_time);
//else
	//	my_sleep_until(MIN_SLEEP_T + philo->current_time);
	return (next_time);
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (check_exit(philo, "main loop start, debug"))
		return (arg);
	printf("main loop thread %d\n", philo->index);
	while (philo->eat_count && !check_exit(philo, "main loop condition"))
	{
		philo->current_time = action(philo);
	}
	return (arg);
}

void	wait_threads(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		printf("thread %d joined\n", i);
		i++;
	}
}

int	main(int ac, char *av[])
{
	t_general	general;

	if (!input(ac, av, &general))
		return (0);
	if (!init_philos(&general))
		return (0);
	if (!intit_threadding(&general))
		return (0);
	wait_threads(&general);
	return (0);
}
// 1703007331949
// 1703007332150
