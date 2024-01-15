/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 18:57:30 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 02:46:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int	ft_strncmp(const char *restrict s1,
	const char *restrict s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n--)
	{
		if (*str1 - *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (0);
}

static inline int	check_death(char *restrict buffer, const int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (buffer[i] == 'd' && !ft_strncmp(buffer + i, "died\n", 5))
			return (i + 5);
		i++;
	}
	return (-1);
}

static inline void	set_exit(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_mutex_lock(&((general->philos + i)->testing_exit_mutext));
		(general->philos + i)->testing_exit = true;
		pthread_mutex_unlock(&((general->philos + i)->testing_exit_mutext));
		i++;
	}
	general->exit = true;
}


# ifndef MAX_STR_NBRS
#  define MAX_STR_NBRS 25
# endif

// numbers must be >= 0
// buffer size is handled by flushing thread
// status mutex must be locked during this
static inline int8_t	put_ints_to_buffer(int64_t cur_time,
	int64_t philo_nb, char *restrict buffer)
{
	int8_t	i;
	int8_t	j;
	int8_t	entered_chars;

	i = MAX_STR_NBRS;
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
	entered_chars = MAX_STR_NBRS - i;
	while (j < entered_chars)
		buffer[j++] = buffer[++i];
	return (entered_chars);
}

// numbers must be >= 0
// buffer size is handled by flushing thread
// status mutex must be locked during this
int8_t	put_output_to_buffer(const int64_t cur_time, const int64_t philo_nb,
		char *restrict buffer, const char *restrict str)
{
	int8_t		entered_chars;

	entered_chars = put_ints_to_buffer(
			(int64_t)(MICROSEC_TO_MILLISEC_FACTOR * cur_time),
			philo_nb, buffer);
	while (*str)
	{
		buffer[entered_chars++] = *str++;
	}
	if (DEBUGING)
		buffer[entered_chars] = 0;
	return (entered_chars);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
	{
		len++;
	}
	return (len);
}

// TODO: figure out a non brute force way to prevent a buffer overflow
void	*flush_output_loop(void *gen)
{
	t_general	*general;
	int			death_index;
	int32_t		operating_size;
	char		*operating_buffer;

	general = (t_general *)gen;
	while (!general->exit)
	{
		usleep(1000);
		__asm__ volatile ("PREFETCHT2 %0" : : "m" (general->buffer_mutex));
		pthread_mutex_lock(&general->buffer_mutex);
		if (!general->output_size)
		{
			pthread_mutex_unlock(&general->buffer_mutex);
			continue;
		}
		operating_size = general->output_size;
		operating_buffer = general->cur_buffer;
		if (operating_buffer == general->output_buffer_a)
			general->cur_buffer = general->output_buffer_b;
		else
			general->cur_buffer = general->output_buffer_a;
		general->output_size = 0;
		pthread_mutex_unlock(&general->buffer_mutex);
		if (general->debug >= 1)
		{
			if (ft_strlen(operating_buffer) != operating_size)
			{
				printf("wrong operating size:\noperating size: %d\nactual: %d\n", operating_size, ft_strlen(operating_buffer));
				usleep(3000000);
			}
		}
		death_index = check_death(operating_buffer, operating_size);
		if (death_index != -1)
		{
			set_exit(general);
			operating_size = death_index;
		}
		write(1, operating_buffer, operating_size);
	}
	return (gen);
}
