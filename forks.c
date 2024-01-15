/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 05:15:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 09:40:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// performence critical
bool	pickup_left_fork(t_philo *restrict const philo)
{
	t_fork				*local_left_fork_ptr;
	pthread_mutex_t		*local_used_mutex_ptr;
	int64_t				usleep_dur;
	int32_t			usleep_buffer;

	local_left_fork_ptr = philo->left_fork;
	local_used_mutex_ptr = &local_left_fork_ptr->mutex_used;
	usleep_buffer = philo->surplus_starve_dur >> 8;
	usleep_dur = usleep_buffer >> 2;
	pthread_mutex_lock(local_used_mutex_ptr);
	while (local_left_fork_ptr->used)
	{
		pthread_mutex_unlock(local_used_mutex_ptr);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (is_dead(philo))
		{
			return (false);
		}
		if (philo->death_t > get_microseconds() + usleep_buffer && usleep_dur > 0)
			usleep(usleep_dur);
		pthread_mutex_lock(local_used_mutex_ptr);
	}
	local_left_fork_ptr->used = true;
	pthread_mutex_unlock(local_used_mutex_ptr);
	pthread_mutex_lock(&local_left_fork_ptr->mutex);
	if (!change_status(philo, PICK_UP_FORK_MSG))
	{
		drop_left_fork(philo);
		return (false);
	}
	return (true);
}

// performence critical
// remove this make 1 pickup fork fucntion
bool	pickup_right_fork(t_philo *restrict const philo)
{
	t_fork			*local_right_fork_ptr;
	int32_t			usleep_dur;
	int32_t			usleep_buffer;

	local_right_fork_ptr = philo->right_fork;
	usleep_buffer = philo->surplus_starve_dur >> 8;
	usleep_dur = usleep_buffer >> 2;
	pthread_mutex_lock(&local_right_fork_ptr->mutex_used);
	while (local_right_fork_ptr->used)
	{
		pthread_mutex_unlock(&local_right_fork_ptr->mutex_used);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
		if (is_dead(philo))
			return (false);
		if (philo->death_t > get_microseconds() + usleep_buffer && usleep_dur > 0)
			usleep(usleep_dur);
		pthread_mutex_lock(&local_right_fork_ptr->mutex_used);
	}
	local_right_fork_ptr->used = true;
	pthread_mutex_unlock(&local_right_fork_ptr->mutex_used);
	pthread_mutex_lock(&local_right_fork_ptr->mutex);
	if (!change_status(philo, PICK_UP_FORK_MSG))
	{
		drop_right_fork(philo);
		return (false);
	}
	return (true);
}

// performence critical
void	drop_right_fork(t_philo *restrict const philo)
{
	pthread_mutex_lock(&philo->right_fork->mutex_used);
	philo->right_fork->used = false;
	pthread_mutex_unlock(&philo->right_fork->mutex_used);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

// performence critical
void	drop_left_fork(t_philo *restrict const philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex_used);
	philo->left_fork->used = false;
	pthread_mutex_unlock(&philo->left_fork->mutex_used);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
}

// performence critical
void	drop_forks(t_philo *restrict const philo)
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
	return ;
}

// performence critical
bool	pickup_forks(t_philo *restrict const philo)
{
	if (!(philo->index % 2))
	{
		
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
			return (false);
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
		{
			return (drop_left_fork(philo), false);
		}
	}
	else
	{
		//usleep(1000);//////////////wtf is this
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->right_fork));
		if (!pickup_right_fork(philo))
			return (false);
		
		__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->left_fork));
		if (!pickup_left_fork(philo))
		{
			return (drop_right_fork(philo), false);
		}
	}
	__asm__ volatile ("PREFETCHT1 %0" : : "m" (philo->death_t));
	return (true);
}
