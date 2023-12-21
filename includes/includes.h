/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:34 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 06:50:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
#define INCLUDES_H

# include <stdbool.h>
# include <stdint.h>

# include <string.h>
/*
	void	*memset(void *b, int c, size_t len);
*/

# include <stdio.h>
/*
	printf
*/

# include <stdlib.h>
/*
	malloc, free
*/

# include <unistd.h>
/*
	write
	int	usleep(useconds_t microseconds); sleepts a trhead for MINIMUM microseconds
*/

# include <sys/time.h>
/*
	struct timeval {
			time_t       tv_sec;    seconds since Jan. 1, 1970 
			suseconds_t  tv_usec;   and microseconds 
	};
	int	gettimeofday(struct timeval *restrict tp, void *restrict tzp);
*/

# include <pthread.h>
/*
	PTHREAD_THREADS_MAX // max threads on system

	int	pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
		returns 0 on successs
		thread ID saved in "thread" variable
		if attr is NULL default is used
		attr can not be changed during runtime
		The thread is created executing start_routine with arg as its sole argument
		If the start_routine returns, the effect is as if there was an implicit call to pthread_exit() using the return value of start_routine as the exit status
		needs to be error hadnled

	when finished:
		Upon thread exit the storage for the thread must be reclaimed by another thread via a call to pthread_join()
		Alternatively, pthread_detach() may be called on the thread to indicate that the system may automatically reclaim the thread storage upon exit

	int	pthread_detach(pthread_t thread);
		good for tasks that do not have to in sync ("fire-and-forget")
		may be called on the thread to indicate that the system may automatically reclaim the thread storage upon exit
		DOES NOT CAUSE TERMINATION
		only ONE call one 1 target
		retuns 0 on success

	int	pthread_join(pthread_t thread, void **value_ptr);
		uspends execution of the calling thread until the target thread terminates unless the target thread has already terminated
		somehow stores the returns values in value_ptr
		does not work on detached threads

	int	pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
		creates a new mutex
		returns 0 on success
		mutex stored in mutex argument
		needs to be error handled

	int	pthread_mutex_destroy(pthread_mutex_t *mutex);
		frees a mutex
		mutex has to be unlocked
		returns 0 on sucess

	int	pthread_mutex_lock(pthread_mutex_t *mutex);
		locks a mutex
		if allready locked the calling thread waits unil its unlocked and then locks it for it self
		returns 0 un success
		gets stuck if a thread locks a mutex and then trys to lock it again

	int	pthread_mutex_unlock(pthread_mutex_t *mutex);
		calling thread MUST hold the lock on the called on mutex
		returns 0 on success
*/

#endif