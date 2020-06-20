#ifndef NMEA_NMEA_H
#define NMEA_NMEA_H

#include <stdint.h>

#define _GPS_MAX_FIELD_SIZE 15
#define _GPS_MAX_DATA_FIELDS 21

typedef struct {
    uint16_t deg;
    uint32_t billionths;
    bool negative;
} RawDegrees;

typedef struct {
    // an additional byte to terminate the c strings
    char identifier[3];
    char sentence[4];
    char data[_GPS_MAX_DATA_FIELDS][_GPS_MAX_FIELD_SIZE + 1];
    uint32_t data_len;
    char checksum[3];
    bool is_valid;
} nmea_message_t;


typedef struct {
    RawDegrees rawLatData, rawLngData;
    bool isValid;
} location_t;

typedef struct {
    location_t location;
    /*TinyGPSDate date;
    TinyGPSTime time;
    TinyGPSSpeed speed;
    TinyGPSCourse course;
    TinyGPSAltitude altitude;
    TinyGPSInteger satellites;
    TinyGPSHDOP hdop; */
} gps_data_t;


enum NMEA_State {
    NMEA_IDENTIFIER,
    NMEA_DATA,
    NMEA_CHECKSUM,
    NMEA_COMPLETE
};

typedef struct {
    gps_data_t data;

    // term parsing
    char term[_GPS_MAX_FIELD_SIZE];
    uint8_t curTermOffset;
    uint8_t parity;
    enum NMEA_State current_state;
    nmea_message_t current_message;
    // statistics
    uint32_t encodedCharCount;
    uint32_t sentencesWithFixCount;
    uint32_t failedChecksumCount;
    uint32_t passedChecksumCount;
    uint32_t failedCount;

} nmea;

bool parseNextChar(nmea *n, char c);
#endif //NMEA_NMEA_H
