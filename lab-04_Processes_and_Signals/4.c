/*
Demonstrate the use of different conversion specifiers and resulting output to allow
the items to be printed.
*/

#include<stdio.h>
#include<unistd.h>

int main(void) {
    char        c  = 'A';
    int         i  = -42;
    unsigned    u  = 42U;
    long        l  = 123456789L;
    float       f  = 3.14159f;
    double      d  = 2.718281828;
    void       *p  = &i;

    char buf[256];
    int n = 0;

    n += sprintf(buf + n, "char        %%c : %c\n",  c);
    n += sprintf(buf + n, "signed int  %%d : %d\n",  i);
    n += sprintf(buf + n, "unsigned    %%u : %u\n",  u);
    n += sprintf(buf + n, "hex int     %%x : %x\n",  i);
    n += sprintf(buf + n, "octal int   %%o : %o\n",  i);
    n += sprintf(buf + n, "long        %%ld: %ld\n", l);
    n += sprintf(buf + n, "float       %%f : %f\n",  f);
    n += sprintf(buf + n, "double sci  %%e : %e\n",  d);
    n += sprintf(buf + n, "double g    %%g : %g\n",  d);
    n += sprintf(buf + n, "pointer     %%p : %p\n",  p);

    write(1, buf, n);
    return 0;
}

/*
output:
char        %c : A
signed int  %d : -42
unsigned    %u : 42
hex int     %x : ffffffd6
octal int   %o : 37777777726
long        %ld: 123456789
float       %f : 3.141590
double sci  %e : 2.718282e+00
double g    %g : 2.71828
pointer     %p : 0x7ffeb0420be8
*/