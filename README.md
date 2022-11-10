## What is OTP RTOS?
OTP (Open Teach Project) is a real-time operating system, the main purpose of developing this operating system is to teach the design of an embedded operating system from scratch.This operating system helps you to jump from theory to practice through clear and simple steps and implement an operating system that has all the necessary facilities on an industrial scale.
Among the multiple benefits that this RTOS provides, there are real-time multithreading, inter-thread communication, synchronization, and memory management. OTP RTOS has many advanced features, including fast interrupt responding by removing critical sections of the kernel, an automatic thread-stack allocator, a stack overflow prediction mechanism, user space inter-thread communication,  relocatable heap memory, and a complete set of system calls.
## Features
 - Supports ARM, RISC-V architectures
 - GCC/LLVM compatibility: ISO/IEC 9899:2011
 - Supports "IEEE-754 standard" coprocessor (ARM)
 - Supports automatic StandBy for low power usage (ARM)
 - System-Call arbitrator (Through atomic instructions)
 - Real-time and definite timer
 - Real-time multicore Load-Balancer (Under development)
 - Non-Blocking kernel structure and Zero-Delay responding to interrupts
 - Supports System-Call from interrupt
 - Stack overflow prediction
 - Kernel synchronization mechanisms (Semaphore, Mutex)
 - Provides accessable Queue and MemPool from interrupt
 - Off-kernel Queue operation
 - Compatibility with Newlib
 - Thread-safe access to heap memory allocator (malloc) through Mutex
 - A new thread-safe and relocatable heap memory (Under development)
## OTP RTOS training course [YouTube Link](https://youtube.com/playlist?list=PLyt95LwGNJJpbJnY5mgZL5yYwBwDt-x7T)
As mentioned above, the main goal of developing this operating system was education. Therefore, a complete video training course has been prepared and in this training course, the implementation of a real-time operating system has been taught from scratch. The topics covered in this training course are listed below.
 - Programming in Linux environment
 - How to use GCC compiler
 - GNU Make
 - GDB (GNU Debugger)
 - ARM Processors architecture and memory model
 - ARM assembly programming
 - Function's stack frame
 - Linking (Relocation and Symbol resolution)
 - Dymanic memory allocation
 - Real-time scheduling algorithms
 - Data structures used in operating systems
 - Kernel and user space in operating systems
 - Thread concept in operating systems
 - Kernel synchronization (Semaphore and Mutex)
 - Inter Thread Communication
 - Memory block allocation (MemPool)
 - Interrupt handling in operating systems
## How To Use OTP RTOS [YouTube Link](https://youtube.com/playlist?list=PLyt95LwGNJJo1ule40yNvtXjbaZ2VU-dq)
This operating system can be easily ported on different platforms. A short training course has been prepared for this purpose and is posted on the OTP YouTube channel, where it is explained how to port this operating system on STM32 microcontrollers.
