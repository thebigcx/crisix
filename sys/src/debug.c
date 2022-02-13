#include <sys/types.h>
#include <sys/libk.h>
#include <sys/arch.h>

#define STACK_CHK_GUARD 0x595e9fbd94fda766

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail()
{
    //panic("Stack smashing detected");
}

#define PORT 0x3f8

void dbgprint(const char *str)
{
    static int ready = 0;
    if (!ready)
    {
        outb(PORT + 1, 0x00);
        outb(PORT + 3, 0x80);
        outb(PORT + 0, 0x03);
        outb(PORT + 1, 0x00);
        outb(PORT + 3, 0x03);
        outb(PORT + 2, 0xc7);
        outb(PORT + 4, 0x0b);
        outb(PORT + 4, 0x1e);
        outb(PORT + 0, 0xae);
        outb(PORT + 4, 0x0f);
        ready = 1;
    }

    for (; *str; str++)
    {
        while ((inb(PORT + 5) & 0x20) == 0);
        outb(PORT, *str);
    }
}

char *strrev(char *str)
{
    int i = strlen(str) - 1, j = 0;

    char c;
    while (i > j)
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
        i--;
        j++;
    }

    return str;
}

int ultoa(unsigned long n, char *str, int base)
{
    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10)
            str[i++] = 'a' + (r - 10);
        else
            str[i++] = '0' + r;

        n = n / base;
    }

    if (i == 0)
        str[i++] = '0';

    str[i] = '\0';
    strrev(str);
    return i;
}

void dbgprintf(const char *format, ...)
{
    va_list list;
    va_start(list, format);

    char strbuf[256];
    char *str = strbuf;

    for (; *format; format++)
    {
        if (*format == '%')
        {
            switch (*(++format))
            {
                case 'd':
                {
                    str += ultoa(va_arg(list, unsigned long), str, 10);
                    break;
                }
            }
        }
        else *str++ = *format;
    }

    *str = 0;

    dbgprint(strbuf);
}