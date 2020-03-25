#include "Teller.h"
#include <stdlib.h>

Teller *new_teller(float idle_time)
{
    Teller *new_teller = malloc(sizeof(Teller));
    new_teller->idle_time = idle_time;

    return new_teller;
}