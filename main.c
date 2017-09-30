#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct reg {
    struct {
        union
        {
            struct
            {
                unsigned char b;
                unsigned char a;
            };
            unsigned short ab;
        };
    };
};

int main()
{
    unsigned short u = 0x4000;
    printf("%x", u);
}