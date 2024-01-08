#include "includes/philo.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>

int main() {

	printf("pointer size: %ld\n", sizeof(void *));
	printf("philo: %ld\n", sizeof(t_philo));
	printf("fork: %ld\n", sizeof(t_fork));
	printf("general: %ld\n", sizeof(t_general));
	printf("pthread_mutex_t: %ld\n", sizeof(pthread_mutex_t));
	return 0;
}

