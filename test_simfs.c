/*
*Name: Julio Alvarenga
*Project 1
*12/1/18
*/
#include "simfs.h"

#define SIMFS_FILE_NAME "simfsFile.dta"

int main()
{
    char *rootFileName = "root";

    if(simfsCreateFileSystem(rootFileName) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    printf("FILE SYSTEM CREATED\n\n");

    //create 5 random files...
    int numOfFiles = 5;
    //holds 5 arrays...
    char *names[numOfFiles];
    for(int i = 0; i < numOfFiles; i++){
        //generate random strings. at index 3, leave the fileName unchanged to create a duplicate file
        char *fileName = (char *)malloc(SIMFS_MAX_NAME_LENGTH * sizeof(char));
        strcpy(fileName,simfsGenerateContent(i+3));

        names[i] = (char *)malloc(SIMFS_MAX_NAME_LENGTH * sizeof(char));
        strcpy(names[i],fileName);

        SIMFS_CONTENT_TYPE file = FILE_CONTENT_TYPE; 

        SIMFS_ERROR er = simfsCreateFile(fileName, file);

        if(er != SIMFS_NO_ERROR){
            PrintError(er);
            //only exit on any error that is NOT a duplicate...
            if(er != SIMFS_DUPLICATE_ERROR)
                exit(EXIT_FAILURE);
        }

    }

    printf("%d FILES CREATED: ", numOfFiles);
    for(int i = 0; i < numOfFiles; i++){
        printf("%s ", names[i]);
    }
    printf("\n\n");

    //getFileInfo and open, write, close and delete files
    for(int i = 0; i < numOfFiles; i++){

        SIMFS_NAME_TYPE fileName_actual;

        //recreates fileNames in the root folder (limitation of testing without FUSE)
        sprintf(fileName_actual, "/%s/", names[i]);

        SIMFS_FILE_DESCRIPTOR_TYPE fd;
        SIMFS_ERROR er = simfsGetFileInfo(fileName_actual, &fd);

        if(er != SIMFS_NO_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }

        printf("FILE INFO %s RECIEVED\n", fd.name);

        //opens the file, creates pcb and passes back a fileHandler index
        SIMFS_FILE_HANDLE_TYPE fh;
        er = simfsOpenFile(fd.name, &fh);
        if(er != SIMFS_NO_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }

        printf("FILE %s OPENED at process index %d\n", fd.name, fh);

        char *buffer = "This goes into the file";
        er = simfsWriteFile(fh, buffer);

        if(er != SIMFS_WRITE_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }

        printf("Wrote: '%s' to File %s\n", buffer, fd.name);

        //blank char pointer, for reading data
        char *read = malloc(SIMFS_DATA_SIZE * sizeof(char));
        read = "unchanged";
        printf("ReadBuffer Before being passed into readFile: %s\n", read);
        er = simfsReadFile(fh, &read);

        if(er != SIMFS_READ_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }

        printf("ReadBuffer After: %s\n", read);

        er = simfsCloseFile(fh);

        if(er != SIMFS_NO_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }
        printf("FILE %s CLOSED\n", fd.name);

        er = simfsDeleteFile(fd.name);

        if(er != SIMFS_NO_ERROR){
            PrintError(er);
            exit(EXIT_FAILURE);
        }
        printf("FILE %s DELETED\n", fd.name);

        printf("Testing if file was deleted. Passing in the same name to getFileInfo\n");
        er = simfsGetFileInfo(fd.name, &fd);
        if(er == SIMFS_NOT_FOUND_ERROR)
            PrintError(er);
    }

    printf("Testing Duplicate FileNames\n\n");

    SIMFS_ERROR error = simfsCreateFile("ab", FILE_CONTENT_TYPE);
    if(error != SIMFS_NO_ERROR)
        PrintError(error);

    SIMFS_FILE_DESCRIPTOR_TYPE fd;
    error = simfsGetFileInfo("/ab/", &fd);
    if(error != SIMFS_NO_ERROR)
        PrintError(error);

    error = simfsCreateFile("ab", fd.type);
    if(error != SIMFS_NO_ERROR)
        PrintError(error);

    printf("Testing Mount and Unmount\n");
    error = simfsMountFileSystem(rootFileName);
    if(error != SIMFS_NO_ERROR){
        PrintError(error);
        exit(EXIT_FAILURE);
    }

    error = simfsUmountFileSystem(rootFileName);
    if(error != SIMFS_NO_ERROR){
        PrintError(error);
        exit(EXIT_FAILURE);
    }
    printf("Mount and Umount return no errors\n");

    // SIMFS_ERROR simfsMountFileSystem(char *simfsFileName);

    /*
        Known Issues: 
        
        -Compiler throws small error due to the way I concatenated the fileName to its path. Code works fine regardless.

        -No integration with FUSE, FUSE mechanics not even considered in this project.

        -Since there are no pid's, there is no test for multiple processes (although the code DOES take into consideration multiple processes).
        This test script is limited to the first rootNode directory, and that fact is reflected in the code.

        -File size not considered. A small segment of code in the createFile addresses the file size dilemma, but no tests were done for
        files large enough to see if it work.
    */


    // the following is just some sample code for simulating user and process identifiers that are
    // needed in the simfs functions
    // int count = 10;
    // char *content;
    // struct fuse_context *context;
    // for (int i = 0; i < count; i++)
    // {
    //     context = simfs_debug_get_context();
    //     printf("user ID = %02i, process ID = %02i, group ID = %02i, umask = %04o\n",
    //            context->uid, context->pid, context->gid, context->umask);
    //     content = simfsGenerateContent(i * 10);
    //     printf("content = \"%s\"\nhash(content) = %ld\n", content, hash((unsigned char *) content));
    // }

    // if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
    //     exit(EXIT_FAILURE);

    // if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
    //     exit(EXIT_FAILURE);

    // if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
    //     exit(EXIT_FAILURE);

    // unsigned char testBitVector[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    // simfsFlipBit(testBitVector, 44);
    // printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    // simfsClearBit(testBitVector, 33);
    // printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    // simfsSetBit(testBitVector, 33);
    // printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));

    return EXIT_SUCCESS;
}
