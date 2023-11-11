#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Print the microseconds part of the current time
    printf("Microseconds: %ld\n", (long)tv.tv_usec);

    return 0;
}
