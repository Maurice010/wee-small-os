# wee-small-os
Simple OS for Raspberry Pi from scratch

## Contents
- [Features](#features)
- [Features](#features)
- [Notes](#notes)
  - [About Memory](#About Memory)
- [Acknowledgements & Useful Resources](#acknowledgements--useful-resources)

## Features

## Notes

### About Memory

**AMD64 Address Translation**

64-bit virtual address format, of which the low-order 48 bits are used in current implementations

63-48 -> Sign extended (either all 0s or all 1s in the upper 16 bits, depending on bit 47, else CPU will raise an exception)
47-39 -> Level 4
38-30 -> Level 3
29-21 -> Level 2
20-12 -> Level 1
11-0 -> Memory page

[Virtual Address bits](https://developer.arm.com/documentation/101811/0103/Translation-granule/The-starting-level-of-address-translation)

Level 1: 4 KB pageframes (Page Table) (512 x 4 KB = 2 MB): Each page table entry points to a 4 KB page frame in physical memory
Level 2: 2 MB blocks (Page Directory) (512 x 2 MB = 1 GB): Each page directory entry points to a page table
Level 3: 1 GB blocks (Page Directory Pointer Table) (512 x 1 GB = 512 GB): Each PDPT entry points to a page directory
Level 4: 512 GB blocks (Page Map Level 4) (512 x 512 GB = 256 TB): Each PML4 entry points to a PDPT

With each additional level of paging, virtual addressing becomes slower.

**ARMv8 Address Translation**

For now, following raspi3-tutorial example, I'm trying to achieve something similar to what was done on AMD64. 

For this OS (again following raspi3-tutorial example) it's worth to mention that:

1. When it comes to Level 4 TTBR0 register holds first entry of PML4 and TTBR1 holds the last entry (we don't have CR3 that points to whole PML4). Due to this ARMv8 addresses between the lower half (managed by TTBR0) and the upper half (managed by TTBR1) are unmapped. This creates a "hole" or range of non-canonical addresses, enforcing separation between different address spaces.

2. All translation tables are stored in a contiguous memory region.

**Memory Attributes**

The memory map of a system can be divided into several regions. Each region can have different memory attributes, such as access permissions that include read and write permissions for different privilege levels, memory type, and cache policies. Memory attributes are configured through a combination of control registers and translation tables.

Translation Tables: For each block or page of virtual addresses, the translation tables provide the corresponding physical address and the attributes for accessing that page. Some memory attributes can be specified in the Table descriptors in higher-level tables. These are hierarchical attributes. This applies to Access Permission, Execution Permissions, and the Physical Address space.

Memory Attribute Indirection Register (MAIR): Defines the encoding of memory attributes used in the translation tables. The MAIR registers allow specifying caching policies, whether memory is normal or device, and bufferability.

System Control Register (SCTLR): Controls the overall settings of the MMU, including enabling/disabling cache, write buffers, and other memory management features.

**Cache attributes**

- Cacheable / Non-cacheable: Defines whether a memory region can be cached. Caching is the act of storing a copy of information from memory into a hardware structure, which is called a cache. The cache is closer to the core and faster for the core to access.

In cacheable regions, data can be managed with different caching policies:
- Write-Through: Writes are immediately written to both the cache and main memory, ensuring data consistency but potentially increasing write latency.
- Write-Back: Writes are only written to the cache initially, and updates to main memory occur later, improving performance but requiring mechanisms to ensure data consistency.

**Shareability**

If a location can be cached, the shareability attributes control which other agents need to see a coherent copy of the memory. Arm expects operating systems to mark the majority of DRAM memory as Normal Write-back cacheable, Inner shareable.

- Inner Shareable: A domain that is shared by other agents, but not necessarily all agent in the system. A system can have several Inner Shareable domains. An operation that affects one Inner Shareable domain does not affect other Inner Shareable domains in the system. It is often used for shared data structures, synchronization primitives, and inter-core communication.
- Outer Shareable: A domain that is shared by multiple agents that can consist of one or more Inner Shareable domains. An operation that affects an Outer Shareable domain also affects all Inner Shareable domains inside it. Outer shareability is crucial for systems that involve multiple processors or clusters, ensuring that all processors can access shared resources.
- Non-Shareable: Memory regions marked as non-shareable can only be accessed by a single processor core.

**Memory access ordering**

Memory access ordering in ARM architecture defines the rules and behaviors for the sequence in which memory operations (loads and stores) are executed and made visible to other components in the system. Memory reordering allows advanced processors to operate more efficiently. It is possible to reorder accesses to locations that are marked Normal, Device-nGRE, or DeviceGRE.

Speculative accesses are memory or data accesses performed by a processor before it is certain that the access is actually needed, based on predicted execution paths. This technique is commonly used in modern CPUs to enhance performance by reducing idle time and improving overall instruction throughput.

- Speculative Instruction Accesses: These involve the CPU predicting and executing instructions along a probable execution path before the CPU confirms that path will be taken.

- Speculative Data Accesses: These involve the CPU preemptively reading from or writing to memory addresses before it is certain that these data accesses will be needed.

Strongly Ordered: Ensures that memory operations occur exactly in the order they are issued by the program. Primarily used for Device Memory or control registers where precise ordering is crucial.

Relaxed (Weak) Ordering: Allows reordering of memory operations for performance optimization. Typical in Normal Memory regions, like system RAM, where the exact order of operations does not impact the correctness of the program.

Device Ordering: Falls between strongly ordered and relaxed ordering. Suitable for Device Memory, such as memory-mapped I/O, where a degree of ordering is required but not as strict as Strongly Ordered.

**Memory types**

All addresses in a system that are not marked as faulting are assigned a memory type. The memory type is a high-level description of how the processor should interact with the address region. The memory type is not directly encoded into the translation table entry. Instead, the AttrIndx field in the translation table entry is used to select an entry from the MAIR_ELx (Memory Attribute Indirection Register).

- Normal Memory: The Normal memory type means that there are no side-effects to the access which means accesses can be out of order. It's used for anything that behaves like a memory, including RAM, Flash, or ROM. Code should only be placed in locations marked as Normal.

- Device Memory: The Device memory type means that there can be side-effects to the access. Used for memory-mapped I/O and other hardware peripherals (eripheral registers are often referred to as Memory-Mapped I/O (MMIO)). Device memory is never cached, and access to it is ordered (speculative data accesses are not permitted) to ensure that reads and writes happen in the expected sequence. 

Instructions should not be placed in regions marked as Device. It's recommend that Device regions are always marked as not executable. Otherwise, it is possible that the processor might speculatively fetch instructions from it, which could cause problems for read-sensitive devices like FIFOs.

Marking a region as Device prevents speculative data accesses (loads and stores) only. Marking a region as non-executable prevents speculative instruction accesses (execution paths). This means that, to prevent any speculative accesses, a region must be marked as both Device and non-executable.

Device Memory is categorized into sub-types that define how the CPU interacts with memory-mapped I/O regions. These sub-types specify different levels of ordering and access restrictions, tailored to the needs of devices and peripherals. The primary sub-types of Device Memory are:
- Device-nGnRnE (Non-Gathering, Non-Reordering, Non-Early Acknowledge)
- Device-nGnRE (Non-Gathering, Non-Reordering, Early Acknowledge)
- Device-nGRE (Non-Gathering, Reordering, Early Acknowledge)
- Device-GRE (Gathering, Reordering, Early Acknowledge)
Each sub-type varies in terms of gathering, reordering, and early-write acknowledgment.

G (Gathering): The CPU can combine multiple memory accesses to similar locations into a single transaction.

R (Reordering): The CPU may reorder memory accesses for optimization.

E (Early Write Acknowledgment): Allows for early acknowledgment of writes, meaning the CPU can continue without waiting for the write to complete - an access can be shown as complete once it is visible to other observers, but before it reaches its destination. For example, a write might become visible to other Processing Elements (PEs) once it reaches a write buffer in the interconnect.

**Permissions**

In a [direct permission scheme](https://developer.arm.com/documentation/102376/0200/Permissions?lang=en), access rights are assigned directly to the entities (e.g., processes, users, or threads).
In an [indirect permission scheme](https://developer.arm.com/documentation/102376/0200/Permissions?lang=en), access rights are not assigned directly to the entities; instead, permissions are granted through an intermediary, such as roles, groups, or policy configurations.

[Access flag](https://developer.arm.com/documentation/102376/0200/Access-Flag?lang=en)

[Dirty state](https://developer.arm.com/documentation/102376/0200/Access-Flag/Dirty-state?lang=en)

## Acknowledgements & Useful Resources
**Raspberry Pi and Other Miscellaneous Documentation**
- [Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/computers/processors.html)
- [Operating Systems Foundations with Linux on the Raspberry Pi](https://www.arm.com/resources/education/books/operating-systems)
- [The mailbox](https://bitbanged.com/posts/understanding-rpi/the-mailbox/)

**Raspberry Pi OS Tutorials**
- [rpi4-osdev](https://github.com/babbleberry/rpi4-osdev/blob/master/part1-bootstrapping/README.md)
- [raspberry-pi-os](https://github.com/s-matyukevich/raspberry-pi-os/tree/master)
- [Building an Operating System for the Raspberry Pi](https://jsandler18.github.io/)
- [raspi3-tutorial](https://github.com/bztsrc/raspi3-tutorial/tree/master)
- [Cambridge Baking Pi](https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/index.html)

**Memory**
- [Virtual Memory Basics Videos](https://youtube.com/playlist?list=PLiwt1iVUib9s2Uo5BeYmwkDFUh70fJPxX&si=sTcvwfu2brzuJkfQ)
- [Paging](https://wiki.osdev.org/Paging)
- [AArch64 memory management guide](https://developer.arm.com/documentation/101811/0103)
- [AArch64 memory attributes and properties](https://developer.arm.com/documentation/102376/0200?lang=en)
- [Memory Systems, Ordering, and Barriers](https://developer.arm.com/documentation/102336/0100)
- [Wikipedia x86-64 article](https://en.wikipedia.org/wiki/X86-64)

**ARM Assembly** 
- [Azeria Introduction to ARM Assembly Basics](https://azeria-labs.com/writing-arm-assembly-part-1/)
- [ARM Assembly By Example](https://armasm.com/)
- [ARM Assembly Video Tutorials](https://www.youtube.com/watch?v=kKtWsuuJEDs&list=PLn_It163He32Ujm-l_czgEBhbJjOUgFhg&index=1)
- [Calling Conventions on ARM](https://stackoverflow.com/questions/55518499/how-to-define-an-arm-assembly-function-that-can-be-used-in-c)
- [A64 ISA Guide](https://developer.arm.com/documentation/102374/0102)
- [How to Use Inline Assembly Language in C Code](https://gcc.gnu.org/onlinedocs/gcc/extensions-to-the-c-language-familyhow-to-use-inline-assembly-language-in-c-code.html)