#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "vsnprintf.h"

#define BUF_SIZE 512

static int snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list args;
    int ret;
    va_start(args, fmt);
    ret = vsnprintf(str, size, fmt, args);
    va_end(args);

    return ret;
}

static int printf(char *fmt, ...)
{
    char buf[BUF_SIZE];
    va_list args;
    int n;
    va_start(args, fmt);
    n = vsnprintf(buf, BUF_SIZE, fmt, args);
    va_end(args);

    if (n < BUF_SIZE)
        n = write(1, buf, n);
    else
        n = write(1, buf, BUF_SIZE - 1);

    return n;
}

int test_vsnprintf()
{
    char buf[BUF_SIZE];
    char *fmt = "Testing a null\n";
    char *str;
    unsigned int n;

    // Passing NULL should give the number of bytes was supposed to write.
    n = snprintf(NULL, 0, fmt);
    assert(n == strlen(fmt));

    // Passing 0 size should give the number of bytes was supposed to write.
    n = snprintf(buf, 0, fmt);
    assert(n == strlen(fmt));

    // Passing a less than required bufsize, should null terminate buffer
    n = snprintf(buf, 7, fmt);
    assert(n == strlen(fmt));
    assert(buf[6] == 0);
   
    // If buf is null, size should not count
    n = snprintf(NULL, 7, fmt);
    assert(n == strlen(fmt));

    n = snprintf(buf, BUF_SIZE, "");
    assert(n == 0);

    // char
    fmt = "Testing %c char\n";
    str = "Testing x char\n";
    n = snprintf(buf, BUF_SIZE, fmt, 'x');
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "Testing %10c char\n";
    str = "Testing          x char\n";
    n = snprintf(buf, BUF_SIZE, fmt, 'x');
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "Testing %-10c char\n";
    str = "Testing x          char\n";
    n = snprintf(buf, BUF_SIZE, fmt, 'x');
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "Test %#-3c\n";
    str = "Test x  \n";
    n = snprintf(buf, BUF_SIZE, fmt, 'x');
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    // string
    fmt = "Testing %s string\n";
    str = "Testing one string\n";
    n = snprintf(buf, BUF_SIZE, fmt, "one");
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "Testing %10s string\n";
    str = "Testing        one string\n";
    n = snprintf(buf, BUF_SIZE, fmt, "one");
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "Testing %-12s string\n";
    str = "Testing one          string\n";
    n = snprintf(buf, BUF_SIZE, fmt, "one");
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "Test a char %-3c and a %10s\n";
    str = "Test a char c   and a     string\n";
    n = snprintf(buf, BUF_SIZE, fmt, 'c', "string");
    assert(n == strlen(str));
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    // numbers
    fmt = "A decimal %d\n";
    str = "A decimal 100\n";
    n = snprintf(buf, BUF_SIZE, fmt, 100);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "A hex %x\n";
    str = "A hex deadc0de\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADC0DE);
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "A zeropad hex %#012X\n";
    str = "A zeropad hex 0X00DEADC0DE\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADC0DE);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "A zeropad hex %012x\n";
    str = "A zeropad hex 0000deadc0de\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADC0DE);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "A zeropad hex %#-012X\n";
    str = "A zeropad hex 0XDEADC0DE  \n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADC0DE);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "An octal %o\n";
    str = "An octal 33653335272\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADBABA);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "A signed %i, %d, %u\n";
    str = "A signed -559039814, -559038242, 3735927482\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xDEADBABA, 0xDEADC0DE, 0xDEADBABA);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "long: %x\n";
    str = "long: deadbabe\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xdeadc0dedeadbabe);
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "long: %lx\n";
    str = "long: deadc0dedeadbabe\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xdeadc0dedeadbabe);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    fmt = "long: %lu\n";
    str = "long: 16045693110842145470\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xdeadc0dedeadbabe);
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "long: %li\n";
    str = "long: -2401050962867406146\n";
    n = snprintf(buf, BUF_SIZE, fmt, 0xdeadc0dedeadbabe);
    assert(strncmp(buf, str, BUF_SIZE) == 0);
    
    fmt = "--%Lb";
    str = "--1101111010101101110000001101111011011110101011011011101010111110";
    n = snprintf(buf, BUF_SIZE, fmt, 0xdeadc0dedeadbabe);
    assert(strncmp(buf, str, BUF_SIZE) == 0);

    return 0;
}

int main()
{
    if (test_vsnprintf() == 0) {
        printf("Test passed!\n");
        return 0;
    }
    printf("Test failed!\n");
    return 1;
}
