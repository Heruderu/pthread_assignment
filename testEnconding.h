//
// Created by helder on 13/02/20.
//

#ifndef CLE_WORDLENGTHS_TESTENCONDING_H
#define CLE_WORDLENGTHS_TESTENCONDING_H

#include <stdio.h>
#include <string.h>

char* test(char *test) {
    unsigned char latinPatternUTF8 = 0xc3;
    unsigned char latinPatternISO8859 = 0xe9;

    long unsigned int length = strlen(test);
    //printf("Length: %lu\n", length);
    for (int i = 0; i < length; i++) {
        if (test[i] == latinPatternUTF8 && length > 1) {
            return "UTF-8";
        }
    }
    if (test[0] == latinPatternISO8859)
        return "ISO-8859-1";

    return 0;
}

#endif //CLE_WORDLENGTHS_TESTENCONDING_H
