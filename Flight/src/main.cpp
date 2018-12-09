#include <Arduino.h>

int main()
{
    setup();
    while(1)
    {
        loop();
        yield();
    }
    return 0;
}