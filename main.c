/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 06:11:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
// if general->debug != DEBUG_NONE the macro DEBUGING
// has to be changed to not 0
t_debug	a(int ac, char *av[])
{
	t_general	general_unalined;
	t_general	*general;

	general = &general_unalined;
	align_ptr((int8_t **)(&general));
	general->debug = DEBUG_INFO;
	if (!input(ac, av, general))
		return (DEBUG_LEAKS);
	if (!init_philos(general))
		return (DEBUG_LEAKS);
	if (!intit_threading(general))
		return (cleanup(general), DEBUG_LEAKS);
	cleanup(general);
	return (general->debug);
}

int	main(int ac, char *av[])
{
	t_debug	debug_mode;

	debug_mode = a(ac, av);
	if (debug_mode == DEBUG_LEAKS)
		system("leaks p");
	return (0);
}
