# file-directory

Simulation of a File System: SIMFS

Built in C and Ubuntu, this script simulates a file directory system using actual memory on the disk.

By allocating a set amount of memory, we can organize that memory into sections called "blocks". Create a file by desginating a free block to hold file information. Within that file exists a reference to another block: a block designated to hold raw data.

A file can be an image, a text, or a folder file. Before creating a file, we look for an available block. If we find a free block, we mark it as "occupied" and set the blocks filename to whatever the user wants. Deleting a file simply marks that block as free. We even cross-check user permission with that block's permission to make sure the user is allowed to modify it.

We can use this rubric to create, update, or delete files. As long as there is enough memory in the system, we can have a file system.
