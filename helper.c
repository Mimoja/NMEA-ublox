#include "helper.h"

// static
// Parse a (potentially negative) number with up to 2 decimal digits -xxxx.yy
int32_t parseDecimal(const char *term) {
    bool negative = *term == '-';
    if (negative) ++term;
    int32_t ret = 100 * (int32_t)atol(term);
    while (isdigit(*term)) ++term;
    if (*term == '.' && isdigit(term[1]))
    {
        ret += 10 * (term[1] - '0');
        if (isdigit(term[2]))
            ret += term[2] - '0';
    }
    return negative ? -ret : ret;
}


void parseDegrees(const char *term, RawDegrees *deg) {
    uint32_t leftOfDecimal = (uint32_t) atol(term);
    uint16_t minutes = (uint16_t)(leftOfDecimal % 100);
    uint32_t multiplier = 10000000UL;
    uint32_t tenMillionthsOfMinutes = minutes * multiplier;

    deg->deg = (int16_t)(leftOfDecimal / 100);

    while (isdigit(*term))
        ++term;

    if (*term == '.')
        while (isdigit(*++term)) {
            multiplier /= 10;
            tenMillionthsOfMinutes += (*term - '0') * multiplier;
        }

    deg->billionths = (5 * tenMillionthsOfMinutes + 1) / 3;
    deg->negative = false;
}

int fromHex(char a)
{
    if (a >= 'A' && a <= 'F')
        return a - 'A' + 10;
    else if (a >= 'a' && a <= 'f')
        return a - 'a' + 10;
    else
        return a - '0';
}