/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 15:33:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

unsigned	ft_atoi(const char *str)
{
	unsigned	num;
	unsigned	digit;

	num = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+')
		str++;
	while (*str >= 48 && *str <= 57)
	{
		num *= 10;
		digit = (int)(*str - '0');
		if (digit < 0)
			digit = -digit;
		num += digit;
		str++;
	}
	return (num);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((long long)time.tv_sec) * 1000 + ((long long)time.tv_usec) / 1000);
}

unsigned long long my_gettime(unsigned long long start_time)
{
	struct timeval s_time;
	unsigned long long time;

	if (gettimeofday(&s_time, NULL)) {
		printf("error getting time\n");
		return 0; // Handle error appropriately
	}
	time = (unsigned long long)s_time.tv_sec * 1000ULL;
	time += (unsigned long long)s_time.tv_usec / 1000ULL;
	time -= start_time;
	return (time);
}

void	my_sleep(unsigned sleep_time)
{
	unsigned long long	target_time;

	target_time = my_gettime(0) + sleep_time;
	while(target_time > my_gettime(0))
	{
	}
}

bool	pickup_left_fork(t_philo *philo)
{
	if (check_exit(philo, "pick up left fork"))
		return (false);
	pthread_mutex_lock(&philo->left_fork->mutex);
	if (check_exit(philo, "pick up left fork"))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (false);
	}
	printf("%llu %d has taken the left fork\n", my_gettime(philo->total_start_t), philo->index);
	return (true);
}

bool	pickup_right_fork(t_philo *philo)
{
	if (check_exit(philo, "pickup right fork"))
		return (false);
	pthread_mutex_lock(&philo->right_fork->mutex);
	if (check_exit(philo, "pickup right fork"))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (false);
	}
	printf("%llu %d has taken the right fork\n", my_gettime(philo->total_start_t), philo->index);
	return (true);
}

bool	drop_forks(t_philo *philo)
{
	if (philo->index % 2)
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	if (check_exit(philo, "drop forks"))
		return (false);
	return (true);
}

bool	check_exit(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->mutex_exit);
	if (*(philo->exit))
	{
		pthread_mutex_unlock(&philo->mutex_exit);
		return (true);
	}
	if (check_starvation(philo, str))
	{
		pthread_mutex_unlock(&philo->mutex_exit);
		return (true);
	}
	pthread_mutex_unlock(&philo->mutex_exit);
	return (false);
}

bool	check_starvation(t_philo *philo, char *str)
{
	unsigned long long	cur_time;

	cur_time = my_gettime(philo->total_start_t);
	if (cur_time > philo->death_time)
	{
		//pthread_mutex_lock(&philo->mutex_exit);
		printf("%llu %d died in %s\n", cur_time, philo->index, str);
		*(philo->exit) = true;
		//pthread_mutex_unlock(&philo->mutex_exit);
		return (true);
	}
	return (false);
}
