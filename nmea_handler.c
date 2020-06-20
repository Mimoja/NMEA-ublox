//
// Created by Mimoja on 20.06.2020.
//

#include <stdio.h>
#include <mem.h>
#include "nmea_handler.h"
#include "helper.h"
#include "nmea.h"


#define ON(X) if(is_valid(m, X))
#define ON_MULT(X,Y) if(is_valid(m, X) && is_valid(m, Y))
#define GET(X) m->data[X]

bool is_valid(nmea_message_t *m, uint32_t data){
    if (data > _GPS_MAX_DATA_FIELDS)
        return false;
    return GET(data)[0] != '\0';
}

bool handle_GGA(gps_data_t *gps, nmea_message_t *m){
    ON(0) printf("TIME: %lld\n", atoll(GET(0)));
    ON(1) parseDegrees(GET(1), &gps->location.rawLatData);
    ON_MULT(1, 2) {
        gps->location.rawLatData.negative = GET(2)[0] == 'S';
        printf("LAT: %c%d.%d\n", gps->location.rawLatData.negative ? '-' : ' ', gps->location.rawLatData.deg, gps->location.rawLatData.billionths);
    }

    ON(3) parseDegrees(GET(3), &gps->location.rawLngData);
    ON_MULT(3, 4) {
        gps->location.rawLngData.negative = m->data[4][0] == 'W';
        printf("LAT: %c%d.%d\n", gps->location.rawLngData.negative ? '-' : ' ', gps->location.rawLngData.deg, gps->location.rawLngData.billionths);
    }
    ON(5)  printf("Qual: %d\n",  atoi(GET(5)));
    ON(6)  printf("Sats used: %d\n", atoi(GET(6)));
    ON(7)  printf("HDOP: %f\n", parseDecimal(GET(7)) / 100.0);
    ON_MULT(8, 9)   printf("Altitude: %f%s\n", parseDecimal(GET(8)) / 100.0, GET(9));
    ON_MULT(10, 11) printf("Separation: %f%s\n", parseDecimal(GET(10)) / 100.0, GET(11));
    ON_MULT(12, 13) printf("Age: %s Stations:%s\n", GET(12), GET(13));
}

bool handle_RMC(gps_data_t *gps, nmea_message_t *m){
    ON(0) printf("TIME: %lld\n", atoll(GET(0)));
    ON(1) printf("Status: %s\n", GET(1));
    ON(2) parseDegrees(GET(2), &gps->location.rawLatData);
    ON_MULT(2, 3) {
        gps->location.rawLatData.negative = GET(3)[0] == 'S';
        printf("LAT: %c%d.%d\n", gps->location.rawLatData.negative ? '-' : ' ', gps->location.rawLatData.deg, gps->location.rawLatData.billionths);
    }

    ON(4) parseDegrees(GET(4), &gps->location.rawLngData);
    ON_MULT(4, 5) {
        gps->location.rawLngData.negative = GET(5)[0] == 'W';
        printf("LAT: %c%d.%d\n", gps->location.rawLngData.negative ? '-' : ' ', gps->location.rawLngData.deg, gps->location.rawLngData.billionths);
    }
    ON(6)  printf("Speed: %f\n",  parseDecimal(GET(6)) / 100.0);
    ON(7)  printf("Course: %f\n",  parseDecimal(GET(7)) / 100.0);
    ON(8)  printf("DATE: %s\n", GET(8));
    ON(9)  printf("MV: %s\n", GET(9));
    ON(10)  printf("MV_EW: %c\n", *GET(10));
    ON(11)  printf("PosMode: %c\n", *GET(11));
    ON(12)  printf("NavStatus: %c\n", *GET(12));
}


bool handle_GLL(gps_data_t *gps, nmea_message_t *m){
    ON(0) parseDegrees(GET(0), &gps->location.rawLatData);
    ON_MULT(0, 1) {
        gps->location.rawLatData.negative = GET(1)[0] == 'S';
        printf("LAT: %c%d.%d\n", gps->location.rawLatData.negative ? '-' : ' ', gps->location.rawLatData.deg, gps->location.rawLatData.billionths);
    }

    ON(2) parseDegrees(GET(2), &gps->location.rawLngData);
    ON_MULT(2, 3) {
        gps->location.rawLngData.negative = m->data[4][0] == 'W';
        printf("LAT: %c%d.%d\n", gps->location.rawLngData.negative ? '-' : ' ', gps->location.rawLngData.deg, gps->location.rawLngData.billionths);
    }
    ON(4) printf("TIME: %lld\n", atoll(GET(4)));
    ON(5) printf("Status: %s\n", GET(5));
    ON(6)  printf("PosMode: %c\n", *GET(6));
}

bool handle_GST(gps_data_t *gps, nmea_message_t *m){
    ON(0) printf("TIME: %lld\n", atoll(GET(0)));
    ON(1)  printf("RMS: %f\n",  parseDecimal(GET(1)) / 100.0);
    ON(2)  printf("StdMaj: %f\n",  parseDecimal(GET(2)) / 100.0);
    ON(3)  printf("StdMin: %f\n",  parseDecimal(GET(3)) / 100.0);
    ON(4)  printf("Orient: %f\n",  parseDecimal(GET(4)) / 100.0);
    ON(5)  printf("StdLat: %f\n",  parseDecimal(GET(5)) / 100.0);
    ON(6)  printf("StdLong: %f\n",  parseDecimal(GET(6)) / 100.0);
    ON(7)  printf("StdAlt: %f\n",  parseDecimal(GET(7)) / 100.0);
}

bool handle_GSV(gps_data_t *gps, nmea_message_t *m){
    int i;
    ON(0) printf("NumMsg: %lld\n", atoll(GET(0)));
    ON(1)  printf("MsgNum: %lld\n",  atoll(GET(1)));
    ON(2)  printf("NumSV: %lld\n",  atoll(GET(2)));
    for (i = 0; i < 4 * 4; i+=4) {
        ON(3 + i) printf("svid: %f\n", parseDecimal(GET(3 + i)) / 100.0);
        ON(4 + i) printf("elv: %f\n", parseDecimal(GET(4 + i)) / 100.0);
        ON(5 + i) printf("az: %f\n", parseDecimal(GET(5 + i)) / 100.0);
        ON(6 + i) printf("cno: %f\n", parseDecimal(GET(6 + i)) / 100.0);
    }
    ON(7 + i) printf("signalId: %f\n", parseDecimal(GET(7 + i)) / 100.0);

}

bool handle_VTG(gps_data_t *gps, nmea_message_t *m){
    ON_MULT(0, 1) printf("cogt: %f%c\n", parseDecimal(GET(0)) / 100.0, *GET(1));
    ON_MULT(2, 3) printf("cogm: %f%c\n", parseDecimal(GET(2)) / 100.0, *GET(3));
    ON_MULT(4, 5) printf("sogn: %f%c\n", parseDecimal(GET(4)) / 100.0, *GET(5));
    ON_MULT(6, 7) printf("sogn: %f%c\n", parseDecimal(GET(6)) / 100.0, *GET(7));
    ON(8)  printf("PosMode: %c\n", *GET(8));
}

bool handle_GSA(gps_data_t *gps, nmea_message_t *m){
    int i;

    ON(0)  printf("OpMode: %c\n", *GET(0));
    ON(1) printf("NavMode: %lld\n", atoll(GET(1)));

    for (i = 0; i < 12; i++) {
        ON(2 + i) printf("Svid: %lld\n", atoll(GET(2 + i)));
    }

    ON(14)  printf("PDOP: %f\n", parseDecimal(GET(14)) / 100.0);
    ON(15)  printf("HDOP: %f\n", parseDecimal(GET(15)) / 100.0);
    ON(16)  printf("VDOP: %f\n", parseDecimal(GET(16)) / 100.0);
    ON(17) printf("SystemID: %lld\n", atoll(GET(17)));

}

nmea_handler_t handler[] = {
        {"GLL", handle_GLL},
        {"GSV", handle_GSV},
        {"GST", handle_GST},
        {"GSA", handle_GSA},
        {"RMC", handle_RMC},
        {"RMB", NULL},
        {"BOD", NULL},
        {"RTE", NULL},
        {"RMM", NULL},
        {"RMZ", NULL},
        {"RME", NULL},
        {"GSA", NULL},
        {"RME", NULL},
        {"VTG", handle_VTG},
        {"GGA", handle_GGA},
};

bool handle_message(nmea *n){
    nmea_handler_t *h;
    nmea_message_t *m = &n->current_message;

    if(!strcmp(m->identifier, "P")){
        printf("Got proprietary Message!\n");
        return false;
    }

    for(int i = 0; i < ARRAY_SIZE(handler); i++) {
        h = &handler[i];
        if (!strcmp(m->sentence, h->sentence)) {
            if (h->handle)
                return h->handle(&n->data, m), true;
            printf("Unimplemented Identifier! '%s'\n", &m->sentence);
            return false;
        }
    }
    printf("Unknown Identifier! '%s'\n", &m->sentence);
    printf("$%s:%s\n", m->identifier, m->sentence);
    for (int i = 0; i < m->data_len; i++) {
        printf("data[%d]: %s\n", i, m->data[i]);
    }
    printf("CHK:%s\n", m->checksum);
}
