/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 21:20:04 by frapp             #+#    #+#             */
/*   Updated: 2023/11/10 22:32:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



int	input(int ac, char *av[], t_general *gen)
{
	if (ac !=5 && ac !=6)
	{
		printf("invalid input ac: %d\n", ac);
		return (0);
	}
	gen->count = ft_atoi(av[1]);
	gen->starve_ti = ft_atoi(av[2]);
	gen->eat_ti = ft_atoi(av[3]);
	gen->sleep_ti = ft_atoi(av[4]);
	if (ac == 6)
		gen->eat_count = ft_atoi(av[5]);
	else
		gen->eat_count = -1;
	if (!gen->count)
	{
		printf("mssing philo count\n");
		return (0);
	}
	return (1);
}

bool	pickup_left_fork(t_philo *philo)
{
	long long	current_time;

	pthread_mutex_lock(&philo->left_fork->mutex_status);
	if (philo->left_fork->status == NUSED)
	{
		philo->left_fork->status = USED;
		pthread_mutex_lock(philo->status_mutex);
		current_time = get_time();
		printf("%lld %d has taken a fork\n", current_time, philo->index);
		pthread_mutex_unlock(philo->status_mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex_status);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->mutex_status);
		thinking
	}
	pthread_mutex_unlock(&philo->left_fork->mutex_status);


	return (true);
}

bool	check_forks(t_fork *left_fork, t_fork *right_fork)
{
	
	return (1);
}

int	eat(t_philo *philo)
{
	long long	eat_time;

	//if (starvation(philo))
	//	return (0);
	//check_forks(philo->left_fork, &(philo->right_fork));
	//if (starvation(philo))
	//	return (0);
	// pthread_mutex_lock(philo->left_fork_p);
	// if (starvation(philo))
	// 	return (0);
	// pthread_mutex_lock(&philo->right_fork);
	pthread_mutex_lock(philo->status_mutex);
	philo->current_time = get_time();
	printf("%lld %d is eating\n", philo->current_time, philo->index);
	pthread_mutex_unlock(philo->status_mutex);
	eat_time = philo->current_time + philo->eat_ti;
	philo->death_time= philo->current_time + philo->starve_ti;
	// gettimeofday(&philo->current_time, NULL);
	while (philo->current_time < eat_time)
	{
		//printf("dif :%lld\n", eat_time - philo->current_time);
		philo->current_time = get_time();
		//if (starvation(philo))
		//	return (0);
		//printf("test %d\n", philo->index);
	}
	// pthread_mutex_unlock(&philo->right_fork);
	// pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	sleep_p(t_philo *philo)
{
	long long	sleep_end;

	pthread_mutex_lock(philo->status_mutex);
	philo->current_time = get_time();
	//if (starvation(philo))
	{
		//return (0);
	}
	printf("%lld %d is sleeping\n", philo->current_time, philo->index);
	pthread_mutex_unlock(philo->status_mutex);
	sleep_end += philo->starve_ti;
	while (philo->current_time < sleep_end)
	{
		//pthread_mutex_lock(philo->status_mutex);
		//if (starvation(philo))
		{
			//pthread_mutex_unlock(philo->status_mutex);
			//return (0);
		}
		//pthread_mutex_unlock(philo->status_mutex);
		philo->current_time = get_time();
	}
	return(1);
}

bool	starvation(t_philo *philo)
{
	pthread_mutex_lock(philo->exit_mutex);
	if (*(philo->exit))
	{
		pthread_mutex_unlock(philo->exit_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->exit_mutex);
	philo->current_time = get_time();
	if (philo->current_time >= philo->death_time)
	{
		//pthread_mutex_lock(philo->exit_mutex);
		(*(philo->exit)) = true;
		pthread_mutex_lock(philo->status_mutex);
		if (check_exit(philo))
			return (1);
		pthread_mutex_lock(philo->exit_mutex);
		*(philo->exit) = true;
		printf("%lld %d died\n", philo->current_time, philo->index);
		pthread_mutex_unlock(philo->exit_mutex);
		pthread_mutex_unlock(philo->status_mutex);
		return (1);
	}
	return (0);
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->death_time = get_time();
	philo->death_time += philo->starve_ti;
	while (philo->eat_count != 0 && !check_exit(philo))
	{
		//printf("main loop\n");
		//printf("eat count: %d\n", philo->eat_count);
		//if (starvation(philo))
		//	return (0);
		if (!eat(philo))
		{
			//return (0);
		}
		if (!sleep_p(philo))
		{
				//return (0);
		}
		if ((int)philo->eat_count > 0)
			philo->eat_count--;
	}
	return (NULL);
}

int	init_threadding(t_general *general)
{
	int		i;
	int		thread_id;
	int		arg;

	i = 0;
	while (i < general->count)
	{
		if (pthread_create(general->threads + i, NULL, main_loop, (general->philos) + i))
		{
			general->threads[i] = NULL;
			return (cleanup(general));
		}
		i++;
	}
	return (1);
}

int	fill_philo(t_general *general, t_philo *philo, int i)
{
	if (i == -1)
		return (1);
	philo->index = i;
	philo->starve_ti = general->starve_ti;
	philo->eat_ti = general->eat_ti;
	philo->sleep_ti = general->sleep_ti;
	philo->eat_count = general->eat_count;
	philo->dead = NUSED;
	philo->exit = &(general->exit);
	philo->exit_mutex = &(general->exit_mutex);
	philo->status_mutex = &(general->status_mutex);
	philo->right_fork = &philo->right_f;
	philo->right_fork->status = false;
	if (pthread_mutex_init(&general->philos[i].right_fork->mutex, NULL))
		return (cleanup(general));
	if (i == general->count - 1 && general->count > 1)
		general->philos[0].left_fork = &general->philos[i].right_fork;
	if (i > 0)
	{
		general->philos[i].left_fork = general->philos[i - 1].right_fork;
		if (pthread_mutex_init(&general->philos[i].right_fork->mutex, NULL))
			return (cleanup(general));
		if (pthread_mutex_init(&general->philos[i].right_fork->mutex_status, NULL))
			return (cleanup(general));
	}
	general->philos[i].thread = general->threads + i;
	return (1);
}

int	init_philos(t_general *general)
{
	int	i;

	if (pthread_mutex_init(&general->exit_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&general->status_mutex, NULL))
		return (0);
	general->threads = NULL;
	general->thread_index = NULL;
	general->philos = NULL;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	if (!general->threads)
		return (cleanup(general));
	general->thread_index = malloc(general->count + 1);
	if (!general->thread_index)
		return (cleanup(general));
	general->philos = malloc(sizeof(t_philo) * (general->count + 1));
	if (!general->philos)
		return (cleanup(general));
	i = -1;
	while (++i < general->count)
	{
		if (!fill_philo(general, general->philos + i, i))
			return (0);
		general->thread_index[i] = i;
	}
	general->threads[general->count] = 0;
	general->thread_index[general->count] = -1;
	general->philos[general->count].index = -1;
	return (1);
}

void wait_threads(t_general *general)
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

	general.exit = false;
	if (!input(ac, av, &general))
		return (0);
	if (!init_philos(&general))
		return (0);
	if (!init_threadding(&general))
		return (0);
	wait_threads(&general);
	return (0);
}