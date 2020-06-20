//
// Created by Mimoja on 20.06.2020.
//

#ifndef NMEA_HELPER_H
#define NMEA_HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "nmea.h"

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
int32_t parseDecimal(const char *term);
void parseDegrees(const char *term, RawDegrees *deg);
int fromHex(char a);

#endif //NMEA_HELPER_H
