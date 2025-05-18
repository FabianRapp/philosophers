# Dining Philosophers Problem – Advanced C Implementation

This project presents a robust and efficient solution to the classic concurrency challenge: the **Dining Philosophers Problem**. Implemented in C, the program leverages multi-threading, with each philosopher represented as an independent thread. The core focus is on preventing deadlocks, ensuring efficient resource management, and maintaining precise timing controls.

## Subject Summary

This project is part of the **42 Network curriculum**, and implements the classic **Dining Philosophers Problem** under strict constraints. The goal is to simulate philosophers eating, thinking, and sleeping in a multi-threaded C program (`philo`), while avoiding starvation, deadlocks, and race conditions. Each philosopher is a thread, and forks are shared resources protected by mutexes. The program must handle precise timing, synchronized logging, and full memory deallocation—without using global variables or any functions outside a handful basic std functions allowed.

While adhering to all project constraints, this implementation goes far beyond the minimum requirements. It introduces advanced synchronization strategies, microsecond-level timing accuracy, and performance optimizations that enable stable simulation with thousands of concurrent threads—all within the strict boundaries defined by the 42 subject.


## Key Features

### 1. Advanced Threading Architecture
Each philosopher runs as a dedicated thread, with their actions—**eating, thinking, sleeping**—executed concurrently. This design closely mirrors real-world parallelism and highlights the complexities of multi-threaded synchronization.

### 2. Deadlock Prevention Mechanism
A sophisticated deadlock prevention strategy is at the heart of the solution:
- Each fork is paired with **dual mutexes**: one for usage status, another for operational control.
- This guarantees mutual exclusion and prevents the classic deadlock scenario by ensuring forks are never held in circular wait conditions.

### 3. Efficient Resource Handling
To maximize performance, the implementation uses:
- **Cache line optimization** and **data prefetching** to minimize cache misses.
- Each philosopher's state and control flags are stored within the same cache line, reducing overhead and boosting execution speed.

### 4. High-Precision Timing Controls
Accurate timing is achieved through:
- Custom timing functions based on `gettimeofday()` and a modified version of `usleep()`.
- A quadratic timing function enables precise management of philosopher actions and robust starvation detection.
- Fine-grained, incremental sleeps achieve microsecond-level precision, even with thousands of threads.

### 5. Starvation Detection & Thread Exit Management
- Starvation is monitored via a shared boolean flag, protected by a mutex.
- This allows efficient and responsive detection with minimal overhead, ensuring reliable simulation and timely thread termination.

### 6. Clean, Scalable Code Structure
- Clear `struct` definitions for philosophers (`t_philo`), forks (`t_fork`), and general settings (`t_general`).
- The codebase is organized for readability, maintainability, and scalability.

---

## Building & Running the Project

**To compile:**
```sh
make
```

**To run:**
```sh
./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [num_times_each_philo_eats]
```

### Example:
```sh
./philo 200 405 200 200
```

**Arguments:**
- `<num_philosophers>`: Number of philosopher threads
- `<time_to_die>`: Time (ms) a philosopher survives without eating
- `<time_to_eat>`: Time (ms) spent eating
- `<time_to_sleep>`: Time (ms) spent sleeping
- `[num_times_each_philo_eats]`: *(optional)* Limit on number of times each philosopher eats (simulation stop condition)

**Example with output redirection:**
```sh
./philo 2000 450 200 200 5 > output
```

---

## Output Format

Each line of output has the following format:
```
<milliseconds_since_start> <philosopher_index> <action>
```
Example:
```
14 871 is thinking
14 871 has taken a fork
14 871 is eating
```
- The first column (**time**) is always sorted, demonstrating proper thread synchronization.  
  *(Note: Even with mutexes, standard `printf` cannot guarantee ordered output for very high thread counts!)*

---

## Performance & Timing Insights

- **Overhead:** With high thread counts (e.g., 2000 philosophers), the average execution window per thread loop is just ~125 microseconds (25 microseconds/core * 5 cores), which is shared with the OS and the huge overhead the 2000 threads give
- **Precision:** Traditional `usleep()` cannot reliably provide such timing granularity due to OS scheduling. Instead, this project uses micro-sleep increments combined with empirical curve fitting to achieve sub-millisecond precision without busy-waiting.
- **Output:** Writing directly to `stdout` from every thread would be too costly. Instead, each philosopher writes to a shared buffer, a dedicated flushing thread swaps it out and flushes it so output—keeping within the project requirement of less than 10 ms delay for output messages is possible for even philo counts far beyond what the subject intended.

---

## Fun Fact

Even with strict synchronization and locking, achieving perfectly ordered, high-volume output is a non-trivial challenge in C duo to kernel buffer managment. To someone not knowing about this this can be very frustraining ^^. This project overcomes those hurdles through custom buffering and timing strategies—resulting in accurate simulation and clear, sequential logs.

---
