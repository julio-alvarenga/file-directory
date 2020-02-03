# file-directory

Simulation of a File System: SIMFS

Built in C and Ubuntu, this script simulates a file directory system using actual memory on the disk.

By allocating a set amount of memory, we can organize that memory into sections called "blocks". There are two types of blocks: a file block and a data block. File blocks contain information like file type, file name, permission, and memory block address. The memory block is a reference to a data block: a block meant to hold raw data.

A file can be an image, a text file, or a folder. Before creating a file, we look for an available block. If we find a free block, we mark it as "occupied" and intialize the block. Deleting a file marks that block as free which can then be used by another file later. We cross-check user permission with each block's permission to make sure the user is allowed to modify it.

We can use this rubric to create, update, or delete files. As long as there is enough memory in the system, we can have a file system.
