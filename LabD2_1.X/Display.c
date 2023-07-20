#include "Display.h"

int numero(int unidad);
int output;

int numero(int unidad){
    switch (unidad){
        case 0:
            output = 0B00111111;
            break;
        case 1:
            output = 0B00000011;
            break;
        case 2:
            output = 0B01011110;
            break;
        case 3:
            output = 0B01001111;
            break;
        case 4:
            output = 0B01100011;
            break;
        case 5:
            output = 0B01101101;
            break;
        case 6:
            output = 0B01111101;
            break;
        case 7:
            output = 0B00000111;
            break;
        case 8:
            output = 0B01111111;
            break;
        case 9:
            output = 0B01100111;
            break;
        case 10:
            output = 0B01110111;
            break;
        case 11:
            output = 0B01111001;
            break;
        case 12:
            output = 0B00111100;
            break;
        case 13:
            output = 0B01011011;
            break;
        case 14:
            output = 0B01111100;
            break;
        case 15:
            output = 0B01110100;
            break;
        default:
            output = 0B00111111;
            break;

    }
    return output;
}