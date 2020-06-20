//
// Created by Mimoja on 20.06.2020.
//

#ifndef NMEA_NMEA_HANDLER_H
#define NMEA_NMEA_HANDLER_H

#include <stdbool.h>
#include "nmea.h"

typedef struct {
    char sentence[4];
    bool (*handle)(gps_data_t *gps, nmea_message_t *m);
} nmea_handler_t;

bool handle_message(nmea *n);

bool handle_GGA(gps_data_t *gps, nmea_message_t *m);
bool handle_GLL(gps_data_t *gps, nmea_message_t *m);
bool handle_RMC(gps_data_t *gps, nmea_message_t *m);

extern nmea_handler_t handler[];

#endif //NMEA_NMEA_HANDLER_H
