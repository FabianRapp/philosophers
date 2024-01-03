/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:53:59 by frapp             #+#    #+#             */
/*   Updated: 2023/12/29 19:26:43 by fabi             ###   ########.fr       */
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

int	cleanup(t_general *general)
{
	int	i;

	if (general->philos && general->threads)
	{
		i = 0;
		pthread_mutex_destroy(&(general->mutex_exit));
		while (i < general->count)
		{
			if (((general->philos) + i)->index >= 0)
				pthread_mutex_destroy(&(((general->philos) + i)->main_fork.mutex));
			if ((general->philos)[i].starve_ti >= 0)
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
