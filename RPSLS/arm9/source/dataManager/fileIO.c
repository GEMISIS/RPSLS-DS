/*
 * A system for opening and closing file buffers.
 * Created by: Gerald McAlister
 */

#include "fileIO.h"

#include <stdio.h>

/*
 * Loads a file into the desired buffer and returns the size of it.
 * @param fileName The name of the file to load.
 * @param buffer The buffer to store the file in.
 * @return Returns the size of the file.
 */
u32 openFile(char* fileName, fileBuffer_t* fileBuffer, int offset)
{
	// Allocate memory for the file name.
	fileBuffer->name = (char*)calloc(strlen(fileName) + 1, sizeof(char));
	// Copy the file name to the file buffer.
	strcpy(fileBuffer->name, fileName);

	// Set the file offset.
	fileBuffer->offset = offset;

	// Open the file to read.
	FILE* f = fopen(fileBuffer->name, "rb");

	// Check that that file was opened successfully.
	if(!f)
	{
		// Return 0 as the file size if the file wasn't opened properly.
		return 0;
	}

	// Seek the end of the file.
	fseek(f, 0, SEEK_END);
	// Set the size to be the position of the end (which is the size f the file).
	fileBuffer->size = ftell(f) - fileBuffer->offset;
	// Rewind the file to the beginnning.
	rewind(f);
	// Set the read position to the current position to read from.
	fseek(f, fileBuffer->offset, SEEK_CUR);

	// Check if there is already a buffer.
	if(fileBuffer->buffer)
	{
		// Free the buffer if there is.
		free(fileBuffer->buffer);
		// Set the buffer to NULL after freeing it.
		fileBuffer->buffer = NULL;
	}
	// Allocate memory for the buffer.
	fileBuffer->buffer = (char*)calloc(fileBuffer->size, sizeof(char));

	// Check if the buffer was setup properly.
	if(fileBuffer->buffer == NULL)
	{
		// Set the size to 0 if it was not.
		fileBuffer->size = 0;
	}
	else
	{
		// Otherwise, read the file into the buffer.
		fread(fileBuffer->buffer, 1, fileBuffer->size - fileBuffer->offset, f);
	}

	// Close the file.
	fclose(f);

	// Return the size of the buffer.
	return fileBuffer->size;
}

/*
 * Unloads a buffer from memory.
 * @param buffer The buffer to close.
 */
void closeBuffer(fileBuffer_t* fileBuffer)
{
	/*
	 * Checks that the buffer is not empty.
	*/
	if(fileBuffer->buffer)
	{
		free(fileBuffer->buffer);
		fileBuffer->buffer = NULL;
	}
}


