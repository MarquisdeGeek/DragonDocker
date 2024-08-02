// The following example program prints the start and end of the memory it uses initially:
// From: http://perso.b2b2c.ca/~sarrazip/dev/cmoc-manual.html

#include <cmoc.h>

int main()
{
    char *s, *e;
    asm
    {   
        leax    program_start,pcr
        stx     :s
        leax    program_end,pcr
        stx     :e
    }
    printf("START AT %p, END AT %p.\n", s, e);
    return 0;
}

