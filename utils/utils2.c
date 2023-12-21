/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:53:59 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 12:28:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

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

// static inline long long	my_gettime(void)
// {
// 	struct timeval s_time;
// 	long long time;

// 	gettimeofday(&s_time, NULL);
// 	//time = (long long)s_time.tv_sec * 1000LL;
// 	time = (long long)s_time.tv_sec << 10;
// 	time += (long long)s_time.tv_usec >> 10;
// 	//time += (long long)s_time.tv_usec / 1000LL;
// 	return (time);
// }