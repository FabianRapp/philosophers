Introduction to the Dining Philosophers Problem Solution in C
Overview

This project presents a sophisticated solution to the classic concurrency problem known as the Dining Philosophers Problem. Implemented in C, the solution employs a multi-threaded approach, where each philosopher is represented as a separate thread, ensuring concurrent execution with a focus on avoiding deadlocks and efficiently managing resources.
Key Features

1. Advanced Threading Mechanism
Each philosopher is treated as an independent entity running in its own thread. This design encapsulates the philosophers' actions (eating, thinking, sleeping) in a concurrent environment, closely mimicking real-world scenarios.

2. Deadlock Prevention Strategy
To address the central challenge of the Dining Philosophers Problem, a sophisticated deadlock prevention mechanism is implemented. Each fork on the table is paired with dual mutexes - one for usage status and another for operational control. This setup prevents deadlocks by ensuring mutual exclusion in fork usage, a crucial aspect of the problem.

3. Efficient Resource Handling
The solution boasts an efficient resource management system. The use of cache lines and data prefetching techniques minimizes cache misses, significantly improving performance. Each philosopher's state, along with essential control flags, is meticulously organized to reside in the same cache line, reducing overhead and enhancing execution speed.

4. Accurate Timing Controls using low level timing functions an mathematics
Custom timing functions, based on gettimeofday() and a modified version of usleep(), are employed together with a quadratic fucntion to manage philosophers' actions with high precision. These functions are critical for accurately simulating the philosophers' behavior and detecting starvation scenarios.

5. Starvation Detection and Thread Exit Management
A shared boolean flag, protected by a mutex, is used for efficient and timely detection of starvation. The solution ensures minimal overhead while checking this flag, crucial for maintaining the responsiveness and reliability of the simulation.

6. Clean and Organized Code Structure
The code is organized with clear struct definitions for philosophers (t_philo) and forks (t_fork), as well as a general struct (t_general) for overarching control. This organization not only aids in understanding the solution but also in maintaining and scaling it.
