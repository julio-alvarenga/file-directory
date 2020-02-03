# file-directory

Simulation of a File System: SIMFS

Built in C and Ubuntu, this script simulates a file directory system using actual memory on the disk.

By allocating a set amount of memory, we can organize that memory into sections called "blocks". Create a file by desginating a free block to hold file information. Within that file exists a reference to another block: a block designated to hold raw data.

A file can be an image, a text, or a folder file.

We can use this rubric to create, update, or delete files. As long as there is enough memory in the system, we can continue to create files.
