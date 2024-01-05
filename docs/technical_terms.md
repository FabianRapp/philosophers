Cache Misses

	A Cache Miss occurs when the data requested by the CPU is not found in the cache. This requires fetching data from slower memory layers, impacting performance.

Cache Line Optimization

	Cache Line Optimization involves organizing data in memory to align with cache line boundaries, maximizing cache utilization and minimizing cache misses.

Struct Packing for Cache Locality

	Struct Packing is the process of organizing data structures to ensure that related data is located close together in memory, improving cache locality and performance.

Data Prefetching

	Data Prefetching is a technique where the CPU anticipates the data it will need in the near future and loads it into the cache in advance.

Inline Assembly to Force Prefetching

	Using Inline Assembly, programmers can insert architecture-specific assembly code within C code to directly control data prefetching.

Threading

	Threading refers to a technique where a process is divided into multiple threads that can be executed concurrently, improving performance in multi-core processors.

Mutex (Mutual Exclusion)

	Mutex is a synchronization primitive used to prevent multiple threads from accessing a shared resource simultaneously, ensuring data integrity.

Concurrency and Concurrent Execution

	Concurrency refers to the ability of a program to execute multiple parts simultaneously, while Concurrent Execution is the execution of these parts in a concurrent manner.

Shared Boolean Flags for Exit Conditions Protected by Mutex

	Shared Boolean Flags are used in threading to signal conditions like program exit, with mutexes ensuring their safe manipulation across threads.

Deadlock Prevention

	Deadlock Prevention strategies involve careful design, such as using two mutexes for one value and a boolean state, to avoid situations where threads are waiting on each other indefinitely.

Data Races and Thread Management

	Data Races occur when multiple threads access shared data concurrently and at least one thread modifies the data.
	Thread Management involves techniques to manage and synchronize access to shared resources.

Heap vs Stack and Their Speed

	Heap and Stack are two types of memory used in C. The stack is used for static memory allocation and is faster, while the heap is used for dynamic memory allocation and is slower.

Memory Leak

	A Memory Leak happens when dynamically allocated memory is not freed properly, leading to a gradual loss of available memory.

Cleanup (Mutexes, Dynamic Data and Threads with Join)

	Cleanup involves deallocating dynamic resources like memory, mutexes, and properly terminating threads using mechanisms like pthread_join in C.

Performance Profiling

	Performance Profiling is the practice of measuring the performance of a program to identify areas for optimization.

Gprof (GNU Profiler)

	Gprof is a performance analysis tool used in Unix-based systems for profiling applications to analyze and identify performance bottlenecks.

C Inline Functions

	C Inline Functions are small functions defined with the inline keyword, suggesting to the compiler to insert their code directly at the point of each function call to reduce overhead.

Branch Prediction

	a CPU process that anticipates the direction of conditional operations, enhancing execution efficiency. Accurate predictions streamline instruction flow, but misses can slow execution due to pipeline corrections.

Optimizing Branch Prediction

	involves arranging code to enhance the predictability of conditional statements, thus reducing pipeline stalls and improving performance.

Basic Idea for opperations costs ( just as a first idea, do not trust these numbers):
	Function Calls: ~1-3 nanoseconds.
	Integer Operations: ~0.5-1 nanoseconds.
	Floating-Point Operations: ~1-10 nanoseconds.
	System Calls: ~500-5000 nanoseconds.
	Mutexes (Locks): ~10-50 nanoseconds (varies with contention).
	Division: ~10-20 nanoseconds
	Multiplication: ~1-3 nanoseconds
	Bit Shifting: 0.5-1 nanoseconds
	Branch Prediction Misses: Slow, causing a delay of 10-20 nanoseconds due to pipeline correction.
	Vector Operations (SIMD), not used in this project yet: Fast, processing multiple data points simultaneously, significantly reducing per-operation time.
