#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include <mem.h>
#include "tests.h"
#include <stdlib.h>
#include "helper.h"
#include "nmea.h"
#include "nmea_handler.h"

bool parseNextChar(nmea *n, char c)
{
    ++n->encodedCharCount;
    switch(c)
    {
        case '\r':
            if (n->current_state == NMEA_CHECKSUM){
                memcpy(&n->current_message.checksum, n->term, 2);
                n->curTermOffset = 0;
                n->current_state = NMEA_COMPLETE;
            } else {
                n->failedCount++;
            }
            break;
        case '\n':
            if (n->parity != 16 * fromHex(n->current_message.checksum[0]) + fromHex(n->current_message.checksum[1])){
                n->failedChecksumCount++;
                n->failedCount++;
                return false;
            }
            n->passedChecksumCount++;
            handle_message(n);
            return true;
        case '$': // sentence begin
            if(!n->current_message.is_valid)
                n->failedCount++;
            n->current_state = NMEA_IDENTIFIER;
            memset(&n->current_message, 0, sizeof(nmea_message_t));
            n->parity = 0;
            n->curTermOffset = 0;
            return false;
        case '*':
            n->current_state = NMEA_CHECKSUM;
        case ',': // term terminators
            n->term[n->curTermOffset] = '\0';
            if (n->current_state == NMEA_IDENTIFIER){
                //TODO assert curTermOffset <= 5
                memcpy(&n->current_message.identifier, n->term, n->curTermOffset - 3);
                memcpy(&n->current_message.sentence, & n->term[n->curTermOffset - 3], 3);
                n->current_state = NMEA_DATA;
            } else {
                char *target = (char *) &n->current_message.data[n->current_message.data_len];
                memcpy(target, n->term, n->curTermOffset + 1);
                n->current_message.data_len++;
            }
            n->curTermOffset  = 0;
            if (n->current_state != NMEA_CHECKSUM)
                n->parity ^= c;
            break;
        default: // ordinary characters
            if (n->curTermOffset < sizeof(n->term) - 1)
                n->term[n->curTermOffset++] = c;
            else
                n->failedCount++;
            if (n->current_state != NMEA_CHECKSUM)
                n->parity ^= c;
            return false;
    }

    return false;
}

int main() {

    nmea n;
    for (int i = 0; i < ARRAY_SIZE(examples); i++){
        for (char *c = &examples[i][0]; *c; c++){
            printf("%c", *c);
            parseNextChar(&n, *c);
        }
    }
    return 0;
}