/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 20:18:43 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 09:39:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "philo.h"

typedef enum e_debug{
	DEBUG_NONE = 0,
	DEBUG_ERROR = 1,
	DEBUG_WARNING = 2,
	DEBUG_INFO = 3,
	DEBUG_VERBOSE = 4,
	DEBUG_LEAKS = 5
}t_debug;

# ifndef DEBUGING
#  define DEBUGING 1
# endif

// used is bool and only int64_t for compiler consistency
// layout exactly counted for cache locality in each function
typedef struct s_fork
{
	int64_t			used; // 72
	pthread_mutex_t	mutex_used; // 136
	pthread_mutex_t	mutex; // 200
}	t_fork;

// layout exactly counted for cache locality in each function
typedef struct s_philo
{
	// cache line 1 (0 - 64)
	pthread_mutex_t		*buffer_mutex; //8
	int64_t				death_t; // 16
	bool				*exit;	// 24
	int64_t				current_t; // 32
	int32_t				*output_size; // 40
	char				**output_buffer; // 48
	t_fork				*left_fork; // 56
	int64_t				sleep_dur; // 64
	// //start 2nd/3rd cache line 64 (stuff the mutex fks with)
	t_fork				main_fork; //192
	// //start 4th cache line 192 (stuff the mutex fks with)
	t_fork				*right_fork; //208
	int64_t				starve_dur; //216
	int64_t				thinking_dur; //224
	int64_t				next_eat_t; //232
	int64_t				eat_count; //240
	int64_t				start_t; //248
	int64_t				index;// 256
	// //start 5th cache line 256:
	int64_t				eat_dur; 
	int64_t				testing_exit;
	int64_t				surplus_starve_dur;
	int64_t				padding3;
	int64_t				padding4;
	int64_t				padding5;
	t_debug				debug;
	int64_t				death_loop;
	//start 5th cache line 320
	pthread_mutex_t		testing_exit_mutext;
	//start 6th cache line 384
}	t_philo;

# ifndef OUTPUT_BUFFER_SIZE
#  define OUTPUT_BUFFER_SIZE 1250000
# endif

// exit, status, output_size and output_buffer
// have to be the first two values to ensure cache locality
typedef struct s_general
{
	bool					exit;
	pthread_mutex_t			buffer_mutex;
	int32_t					output_size;
	char					*cur_buffer;
	char					output_buffer_a[OUTPUT_BUFFER_SIZE];
	char					output_buffer_b[OUTPUT_BUFFER_SIZE];
	t_philo					*philos;
	pthread_t				*threads;
	int32_t					count;
	int32_t					starve_dur;
	int32_t					eat_dur;
	int32_t					sleep_dur;
	int32_t					eat_count;
	int64_t					start_t;
	t_philo					*ptr_to_free_philos;
	bool					even;
	bool					death_loop;
	t_debug					debug;
	int8_t					alignment_buffer_dont_use[64];
}	t_general;

#endif