/**
 * Copyright (c) 2017 elttam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __x86_64__
#define canary_t     uint64_t
#define INSN_READ    "movq %%fs:0x28, %0;"
#define FMT          "Found canary: %#lx\n"

#elif __i386__
#define canary_t     uint32_t
#define INSN_READ    "movl %%gs:0x14, %0;"
#define FMT          "Found canary: %#x\n"
#endif


canary_t read_canary()
{
        canary_t val = 0;

        __asm__(INSN_READ
                : "=r"(val)
                :
                :);

        return val;
}


void  greetz(char* name)
{
        char buf[64] = {0, };
        printf(FMT, read_canary());
        strcpy(buf, name);                            /* BoF is on purpose :) */
        printf("Hello %s\n", buf);
        return;
}



int main(int argc, char** argv, char** envp)
{
        int pid;

        if(argc!=2){
                printf("Missing argument\n");
                return EXIT_FAILURE;
        }

        greetz(argv[1]);
        return EXIT_SUCCESS;
}
