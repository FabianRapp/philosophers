/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 05:15:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/06 12:43:03 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static inline int64_t __attribute__((always_inline))
	get_microseconds_forks(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (((int64_t)s_time.tv_sec) * 1000000 + s_time.tv_usec);
}

static inline bool __attribute__((always_inline))
	check_exit_forks(t_philo *restrict const philo)
{
	bool			*local_ptr;
	pthread_mutex_t	*local_mutex_ptr;

	local_ptr = philo->exit;
	local_mutex_ptr = philo->status;
	philo->current_t = get_microseconds_forks();
	pthread_mutex_lock(local_mutex_ptr);
	if (!(*local_ptr))
	{
		pthread_mutex_unlock(local_mutex_ptr);
		if (philo->current_t <= philo->death_t)
		{
			return (false);
		}
		return (do_exit(philo, false));
	}
	return (do_exit(philo, true));
}

bool	pickup_left_fork(t_philo *restrict const philo)
{
	t_fork				*local_left_fork_ptr;
	pthread_mutex_t		*local_used_mutex_ptr;

	local_left_fork_ptr = philo->left_fork;
	local_used_mutex_ptr = &local_left_fork_ptr->mutex_used;
	pthread_mutex_lock(local_used_mutex_ptr);
	while (local_left_fork_ptr->used)
	{
		pthread_mutex_unlock(local_used_mutex_ptr);
		__asm__ volatile ("PREFETCHT0 %0" : : "m" (philo->death_t));
		if (check_exit_forks(philo))
			return (false);
		pthread_mutex_lock(local_used_mutex_ptr);
	}
	local_left_fork_ptr->used = true;
	pthread_mutex_unlock(local_used_mutex_ptr);
	philo->current_t = get_microseconds();
	pthread_mutex_lock(&local_left_fork_ptr->mutex);
	if (!change_status(philo, PICK_UP_FORK_MSG))
	{
		drop_left_fork(philo);
		return (false);
	}
	return (true);
}

bool	pickup_right_fork(t_philo *restrict const philo)
{
	t_fork	*local_right_fork_ptr;

	local_right_fork_ptr = philo->right_fork;
	pthread_mutex_lock(&local_right_fork_ptr->mutex_used);
	while (local_right_fork_ptr->used)
	{
		pthread_mutex_unlock(&local_right_fork_ptr->mutex_used);
		__asm__ volatile ("PREFETCHT0 %0" : : "m" (philo->exit));
		if (check_exit_forks(philo))
			return (false);
		pthread_mutex_lock(&local_right_fork_ptr->mutex_used);
	}
	local_right_fork_ptr->used = true;
	pthread_mutex_unlock(&local_right_fork_ptr->mutex_used);
	philo->current_t = get_microseconds();
	pthread_mutex_lock(&local_right_fork_ptr->mutex);
	if (!change_status(philo, PICK_UP_FORK_MSG))
	{
		drop_right_fork(philo);
		return (false);
	}
	return (true);
}

void	drop_right_fork(t_philo *restrict const philo)
{
	pthread_mutex_lock(&philo->right_fork->mutex_used);
	philo->right_fork->used = false;
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
}

void	drop_left_fork(t_philo *restrict const philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex_used);
	philo->left_fork->used = false;
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
}

bool	drop_forks(t_philo *restrict const philo)
{
	if (!(philo->index % 2))
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		drop_left_fork(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		drop_right_fork(philo);
	}
	else
	{
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		drop_right_fork(philo);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		drop_left_fork(philo);
	}
	//__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->exit));
	if (check_exit_forks(philo))
		return (false);
	return (true);
}
