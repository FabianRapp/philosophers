/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:53:59 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 17:06:29 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

D_TYPE	ft_atoi(const char *str)
{
	D_TYPE	num;
	D_TYPE	sign;
	D_TYPE	digit;

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
		digit = (int64_t)(*str - '0');
		if (digit < 0)
			digit = -digit;
		num += digit;
		str++;
	}
	return (num * sign);
}

int	cleanup(t_general *general)
{
	int	i;

	if (general->philos && general->threads)
	{
		i = 0;
		pthread_mutex_destroy(&(general->status));
		while (i < general->count)
		{
			if (((general->philos) + i)->eat_dur != MUTEX_FAIL_CHECK_VAL)
				pthread_mutex_destroy(&(((general->philos) + i)->main_fork.mutex));
			if ((general->philos)[i].starve_dur != MUTEX_FAIL_CHECK_VAL)
				pthread_mutex_destroy(&(((general->philos) + i)->main_fork.mutex_used));
			i++;
		}
	}
	if (general->philos)
		free(general->philos);
	general->philos = NULL;
	if (general->threads)
		free(general->threads);
	general->threads = NULL;
	return (0);
}
