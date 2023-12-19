/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 15:42:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
TODO:
	- handle 1 philo only
	- handle early error returns and cleanup
*/

int	eat(t_philo *philo)
{
	unsigned long long		current_time;
	static __thread unsigned int		eat_cout = 0; //debugging

	if (check_exit(philo, "start eat"))
		return (0);
	printf("%llu %d is thinking\n", my_gettime(philo->total_start_t), philo->index);
	if (philo->index % 2)
	{
		if (!pickup_left_fork(philo))
			return (0);
		if (!pickup_right_fork(philo))
			return (0);
	}
	else
	{
		if (!pickup_right_fork(philo))
			return (0);
		if (!pickup_left_fork(philo))
			return (0);
	}
	current_time = my_gettime(philo->total_start_t);
	printf("%llu %d is eating for the %u. time\n", my_gettime(philo->total_start_t), philo->index, ++eat_cout);
	my_sleep(philo->eat_ti);
	if (!drop_forks(philo))
		return (0);
	philo->death_time = current_time + philo->starve_ti;
	if (philo->eat_count > 0)
		philo->eat_count--;
	printf("%llu %d is sleeping\n", my_gettime(philo->total_start_t), philo->index);
	my_sleep(philo->sleep_ti);
	return (1);
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("main loop thread %d\n", philo->index);
	pthread_mutex_lock(&philo->mutex_exit);
	while (!*(philo->exit))
	{
		pthread_mutex_unlock(&philo->mutex_exit);
		if (!philo->eat_count)
			return (arg);
		eat(philo);
		pthread_mutex_lock(&philo->mutex_exit);
	}
	pthread_mutex_unlock(&philo->mutex_exit);
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
