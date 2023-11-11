/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 01:07:57 by frapp             #+#    #+#             */
/*   Updated: 2023/11/09 05:14:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

static int	white_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

unsigned	ft_atoi(const char *str)
{
	unsigned	num;
	unsigned	digit;

	num = 0;
	while (white_space(*str))
		str++;
	if (*str == '+')
		str++;
	while (ft_isdigit(*str))
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
