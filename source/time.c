#include "../include/time.h"

void GetMonth(int type) {
    switch(type) {
        case 0:
            printf("Jan ");
            break;
        case 1:
            printf("Feb ");
            break;
        case 2:
            printf("Mar ");
            break;
        case 3:
            printf("Apr ");
            break;
        case 4:
            printf("May ");
            break;
        case 5:
            printf("Jun ");
            break;
        case 6:
            printf("Jul ");
            break;
        case 7:
            printf("Aug ");
            break;
        case 8:
            printf("Sep ");
            break;
        case 9:
            printf("Oct ");
            break;
        case 10:
            printf("Nov ");
            break;
        case 11:
            printf("Dec ");
            break;
    }
}

void GetWeek(int type) {
    switch (type) {
        case 0:
            printf("Sun ");
            break;
        case 1:
            printf("Mon ");
            break;
        case 2:
            printf("Tue ");
            break;
        case 3:
            printf("Wed ");
            break;
        case 4:
            printf("Thu ");
            break;
        case 5:
            printf("Fri ");
            break;
        case 6:
            printf("Sat ");
            break;
    }
    
}