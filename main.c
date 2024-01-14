/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/14 03:00:18 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
int	a(int ac, char *av[])
{
	t_general	general_unalined[3];
	t_general	*general;

	general = general_unalined;
	align_ptr((int8_t **)(&general));
	general->debug = DEBUG_INFO;
	if (!input(ac, av, general))
		return (0);
	if (!init_philos(general))
		return (0);
	if (!intit_threading(general))
		return (0);
	cleanup(general);
	return (0);
}

int	main(int ac, char *av[])
{
	a(ac, av);
	//system("leaks p");
	return (0);
}
