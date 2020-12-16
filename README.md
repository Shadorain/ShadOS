# ShadOS Kernel
## Basic Kernel

## Adding a keyboard driver
How does our computer speak with our I/O devices (keyboard, mouse, etc)? Simple: I/O ports!
- I/O ports are specific addresses on the architecture's I/O bus.
- The read/write instructions are already built into processor, we just have to use them to build our ports!
  - `in` and `out` are the instructs for this specific architecture
