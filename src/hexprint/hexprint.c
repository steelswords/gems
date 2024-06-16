/* Copyright (C) 2023 Tristan Andrus ("steelswords")
 * 
 *   Modified Zlib License
 * 
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 * 
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 *   4. You must send the author, Tristan Andrus, a message saying you used his
 *      code, to make him feel useful.
 * 
 *   Tristan Andrus
 *   tristan <at> tristanandrus <dot> com
 */

#include "hexprint.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/** @brief The width of rows hexPrint will print. */
#define HEXPRINT_ROW_WIDTH 8

void hexPrint(uint8_t* source, uint32_t size);
void hexPrint_printAddress(void* pointer);
char hexPrint_getAsciiRepresentation(char data);

/** Dumps memory like so:
 * [0x12345670] 00 01 02 03 04 05 06 07 | asciirepresentan
 * [0x12345678] 08 09 0a 0b 0c 0d 0e 0f | asciirepresentan
 * [0x12345680] 00 01 02 03 04 05 06 07 | asciirep...entan
 */
void hexPrint(uint8_t* source, uint32_t size)
{
    // Print the first address
    //char asciiBuffer[8] = {0,0,0,0, 0,0,0,0};
    char asciiBuffer[HEXPRINT_ROW_WIDTH + 1] = {0}; // +1 character for null termination.
    size_t asciiBufferIndex = 0;
    
    for (uint32_t i = 0; i < size; ++i)
    {
        //If we're at the beginning of the row, print the address
        if (i % HEXPRINT_ROW_WIDTH == 0)
        {
            hexPrint_printAddress(&source[i]);
        }
        
        //Print the data at the current address
        printf("%02x ", source[i]);

        //Add the ASCII interpretation to the asciiBuffer
        asciiBuffer[asciiBufferIndex] = hexPrint_getAsciiRepresentation(source[i]);
        asciiBufferIndex++;

        // If we've printed 8 digits, print the asciiBuffer and then the newline
        // If we're 1 before the wrapparound (mod == 0), time to print the end of the row.
        if (i % HEXPRINT_ROW_WIDTH == (HEXPRINT_ROW_WIDTH - 1))
        {
            printf("| %s\n", asciiBuffer);
            memset(asciiBuffer, 0, HEXPRINT_ROW_WIDTH + 1);
            asciiBufferIndex = 0;
        }
        // If we're at the end of the buffer, add padding and print the end anyway.
        else if (i + 1 == size)
        {
            size_t numberOfSpacesToPad = HEXPRINT_ROW_WIDTH - 1 - (i % HEXPRINT_ROW_WIDTH);
            for (size_t j = 0; j < numberOfSpacesToPad; ++j)
            {
                printf("   ");
            }
            printf("| %s\n", asciiBuffer);
            memset(asciiBuffer, 0, HEXPRINT_ROW_WIDTH + 1);
            asciiBufferIndex = 0;
        }
    }
}


void hexPrint_printAddress(void* pointer)
{
    char formatString[16] = "[0x%016lx] ";

    // If we're on a 32-bit platform, only print 8 characters (4 bytes)
    if (sizeof(uintptr_t) == 4)
    {
        // A brief search through documentation did not yield a better indicator
        // of us running on a Raspberry Pi Pico
#ifdef _PICO_STDIO_H
        // The format options on the Pico are more simple.
        strcpy(formatString, "[0x%08x] ");
#else
        strcpy(formatString, "[0x%08lx] ");
#endif
    }
    printf(formatString, (uintptr_t)pointer);
}

char hexPrint_getAsciiRepresentation(char data)
{
    if (' ' <= data && data <= '~')
    {
        return data;
    }
    return '.';
}
