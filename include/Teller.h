#ifndef _TELLER_H_
#define _TELLER_H_

/* max idle time is 10 mins */
#define MAX_IDLE_TIME 10.0 

typedef struct Teller
{
    int id;
    float idle_time;
} Teller;

/* Creates a new teller with idle_time */
Teller *new_teller(float idle_time);

#endif