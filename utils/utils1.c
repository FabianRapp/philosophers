/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:49:20 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 19:24:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	white_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;
	int	digit;

	num = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
	{
		str++;
		sign = -1;
	}
	else if (*str == '+')
		str++;
	while ( (*str >= 48 && *str <= 57))
	{
		num *= 10;
		digit = (int)(*str - '0');
		if (digit < 0)
			digit = -digit;
		num += digit;
		str++;
	}
	return (num * sign);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((long long)time.tv_sec) * 1000 + ((long long)time.tv_usec) / 1000);
}

long long	my_gettime(void)
{
	struct timeval s_time;
	long long time;

	gettimeofday(&s_time, NULL);
	time = (long long)s_time.tv_sec * 1000LL;
	//time += (long long)s_time.tv_usec >> 10;
	time += (long long)s_time.tv_usec / 1000LL;
	return (time);
}

void	my_sleep_until(long long target_time)
{
	while(my_gettime() < target_time)
	{
	}
}

bool	pickup_left_fork(t_philo *philo)
{
	if (check_exit(philo, "pick up left fork"))
		return (false);
	pthread_mutex_lock(&philo->left_fork->mutex);
	printf("%lld %d has taken the left fork\n", my_gettime() - philo->total_start_t, philo->index);
	if (check_exit(philo, "pick up left fork"))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (false);
	}
	return (true);
}

bool	pickup_right_fork(t_philo *philo)
{
	if (check_exit(philo, "pickup right fork"))
		return (false);
	pthread_mutex_lock(&philo->right_fork->mutex);
	printf("%lld %d has taken the right fork\n", my_gettime() - philo->total_start_t, philo->index);
	if (check_exit(philo, "pickup right fork"))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (false);
	}
	return (true);
}

long long	eat(t_philo *philo)
{
	static __thread int		eat_cout = 0; //debugging

	if (philo->index % 2)
	{
		if (!pickup_left_fork(philo))
			return (0);
		if (!pickup_right_fork(philo))
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			return (0);
		}
	}
	else
	{
		if (!pickup_right_fork(philo))
			return (0);
		if (!pickup_left_fork(philo))
		{
			pthread_mutex_unlock(&philo->right_fork->mutex);
			return (0);
		}
	}
	philo->current_time = my_gettime();
	//printf("time left philo %d: %lld\n", philo->index, (long long)philo->death_time - (long long)philo->current_time);
	printf("%lld %d is eating for the %d. time\n", my_gettime() - philo->total_start_t, philo->index, ++eat_cout);
	if (check_exit(philo, "end of eating"))
		return (0);
	my_sleep_until(philo->current_time + philo->eat_ti);
	philo->death_time = philo->current_time + philo->starve_ti;
	if (!drop_forks(philo))
		return (0);
	return (philo->current_time + philo->eat_ti);
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

// bool	check_exit(t_philo *philo, char *str)
// {
// 	pthread_mutex_lock(philo->mutex_exit);
// 	if (*(philo->exit))
// 	{
// 		pthread_mutex_unlock(philo->mutex_exit);
// 		return (true);
// 	}
// 	pthread_mutex_unlock(philo->mutex_exit);
// 	if (check_starvation(philo, str))
// 	{
// 		return (true);
// 	}
// 	return (false);
// }

bool	check_exit(t_philo *philo, char *str)
{
	long long	local_current_time;

	local_current_time = my_gettime();
	pthread_mutex_lock(philo->mutex_exit);
	if (!(*(philo->exit)))
	{
		if (!(local_current_time >= philo->death_time))
		{
			pthread_mutex_unlock(philo->mutex_exit);
		}
		else
		{
			printf("%lld %d died in %s\n", philo->current_time - philo->total_start_t, philo->index, str);
			*(philo->exit) = true;
			pthread_mutex_unlock(philo->mutex_exit);
			return (true);
		}
		return (false);
	}
	pthread_mutex_unlock(philo->mutex_exit);
	return (true);
}
