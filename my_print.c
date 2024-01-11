/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 18:57:30 by frapp             #+#    #+#             */
/*   Updated: 2024/01/12 00:34:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

#  define MAX_NUMBERS_LEN 15
// numbers must be >= 0
// buffer size is handled by flushing thread
// status mutex must be locked during this
int8_t	put_ints_to_buffer(int64_t cur_time, int64_t philo_nb, char *buffer)
{
	int8_t	i;
	int8_t	j;
	int8_t	entered_chars;

	i = MAX_NUMBERS_LEN;//this is walys enough space for the entered numbers and spaces
	if (!philo_nb)
		buffer[i--] = '0';
	while (philo_nb > 0)
	{
		buffer[i--] = (philo_nb % 10) + '0';
		philo_nb /= 10;
	}
	buffer[i--] = ' ';
	if (!cur_time)
		buffer[i--] = '0';
	while (cur_time > 0)
	{
		buffer[i--] = cur_time % 10 + '0';
		cur_time /= 10;
	}
	j = 0;
	entered_chars = MAX_NUMBERS_LEN - i;
	while (j < entered_chars)
		buffer[j++] = buffer[++i];
	return (entered_chars);
}

// numbers must be >= 0
// buffer size is handled by flushing thread
// status mutex must be locked during this
int8_t	put_output_to_buffer(int64_t cur_time, int64_t philo_nb,
		char *buffer, const char *restrict str)
{
	int8_t		entered_chars;

	entered_chars = put_ints_to_buffer(
			(int64_t)(MICROSEC_TO_MILLISEC_FACTOR * cur_time), philo_nb, buffer);
	while(*str)
	{
		buffer[entered_chars++] = *str++;
	}
	return (entered_chars);
}

// buffer size will always be a size that is not possibly fully filled at this point
void	*flush_output_loop(void *gen)
{
	t_general	*general;
	int64_t		interval;

	general = (t_general *)gen;
	interval = 1000;
	while (1)
	{
		pthread_mutex_lock(&general->status_mutex);
		if (general->output_size)
			write(1, general->output_buffer, general->output_size);
		general->output_size = 0;
		if (!general->exit)
		{
			pthread_mutex_unlock(&general->status_mutex);
		}
		else
		{
			pthread_mutex_unlock(&general->status_mutex);
			return (gen);
		}
		usleep(interval);
		__asm__ volatile ("PREFETCHT2 %0" : : "m" (general->status_mutex));
	}
}
