/*
 * A system for opening and closing file buffers.
 * Created by: Gerald McAlister
 */

#ifndef _FILE_BROWSER_H_
#define _FILE_BROWSER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>

// A file buffer type for reading files.
typedef struct
{
	char* name;
	char* buffer;
	unsigned int size;
	unsigned int offset;
}fileBuffer_t;

/*
 * Loads a file into the desired buffer and returns the size of it.
 * @param fileName The name of the file to load.
 * @param fileBuffer The buffer to store the file in.
 * @param offset The offset to start reading the file at.
 * @return Returns the size of the file.
 */
extern u32 openFileBuffer(char* fileName, fileBuffer_t* fileBuffer, int offset);

/*
 * Unloads a buffer from memory.
 * @param fileBuffer The buffer to close.
 */
void closeBuffer(fileBuffer_t* fileBuffer);

#ifdef __cplusplus
}
#endif

#endif
