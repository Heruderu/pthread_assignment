//
// Created by helder on 18/02/20.
//

#ifndef CLE_WORDLENGTHS_READER_H
#define CLE_WORDLENGTHS_READER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAXINT 15000

bool isntSeparator(unsigned int c, bool thirdByte);

bool isApostropheOrSingleQuote(unsigned int c, bool thirdByte);

bool isThreeBytePunctuation(unsigned int c);

bool isAlphaNumericOrUnderscore(unsigned int c);

int find_maximum(const int countArray[], int numWords);

void countLengths(int *lengthArray, int *vowelFrequencies, int const *wordLengths, int const *vowelCounts,
        int maxLength, int numWords);

int reader(char file[], char encoding[]) {
    FILE *filePtr;
    int wordLengths[MAXINT];
    int vowelCounts[MAXINT];
    unsigned char twoByte = 0xc0, threeByte = 0xe0, fourByte = 0xf0;
    unsigned char latinPatternUTF8 = 0xc3;
    unsigned char ptSpecialVowelsArray[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa8,
                                            0xa9, 0xaa, 0xac, 0xad, 0xb2,
                                            0xb3, 0xb4, 0xb5, 0xb9, 0xba,
                                            0x80, 0x81, 0x82, 0x83, 0x88,
                                            0x89, 0x8a, 0x8c, 0x8d, 0x92,
                                            0x93, 0x94, 0x95, 0x99, 0x9a, '\0'};
    unsigned char vowelsArray[] = {0x41, 0x45, 0x49, 0x4f, 0x55,
                                   0x61, 0x65, 0x69, 0x6f, 0x75, '\0'};
    unsigned char cWithCedillaLower = 0xa7, cWithCedillaUpper = 0x87;
    unsigned int c = 0;
    int vowelCounter = 0;
    int counter = 0;
    int wordCounter = 0;
    int positionWordCounts = 0;

    if ((filePtr = fopen(file, "r")) == NULL) {
        printf("Error reading file!\n");
        return EXIT_FAILURE;
    } else {
        if (strncmp(encoding, "UTF-8", strlen(encoding)) == 0) {
            while (1) {
                if (feof(filePtr)) {
                    break;
                }
                c = fgetc(filePtr);
                if ((c & fourByte) == fourByte) {
                    for (int i = 0; i < 3; i++) {
                        fgetc(filePtr);
                    }
                    continue;
                } else if ((c & threeByte) == threeByte) {
                    for (int i = 0; i < 2; i++) {
                        c = fgetc(filePtr);
                    }
                    if (isntSeparator(c, true) || isThreeBytePunctuation(c)) {
                        continue;
                    } else if (counter != 0) {
                        wordLengths[positionWordCounts++] = counter;
                        wordCounter++;
                        counter = 0;
                    }
                    continue;
                } else if ((c & twoByte) == twoByte) {
                    if (isntSeparator(c, false)) {
                        if (c == latinPatternUTF8) {
                            c = fgetc(filePtr);
                            for (int i = 0; ptSpecialVowelsArray[i] != '\0'; i++) {
                                if (c == ptSpecialVowelsArray[i]) {
                                    vowelCounter++;
                                    counter++;
                                    break;
                                }
                            }
                            if (c == cWithCedillaLower || c == cWithCedillaUpper) {
                                counter++;
                            }
                        } else {
                            fgetc(filePtr);
                            continue;
                        }
                    } else if (counter != 0) {
                        vowelCounts[positionWordCounts] = vowelCounter;
                        wordLengths[positionWordCounts++] = counter;
                        wordCounter++;
                        counter = 0;
                        vowelCounter = 0;
                    }
                    continue;
                } else {
                    if (isntSeparator(c, false)) {
                        for (int i = 0; vowelsArray[i] != '\0'; i++) {
                            if (c == vowelsArray[i]) {
                                vowelCounter++;
                                break;
                            }
                        }
                        if (isApostropheOrSingleQuote(c, false)) {
                            continue;
                        } else {
                            if (isAlphaNumericOrUnderscore(c)) {
                                counter++;
                            } else {
                                continue;
                            }
                        }
                    } else if (counter != 0) {
                        vowelCounts[positionWordCounts] = vowelCounter;
                        wordLengths[positionWordCounts++] = counter;
                        wordCounter++;
                        counter = 0;
                        vowelCounter = 0;
                    }
                }
            }
        }

/*        for (int j = 0; wordLengths[j] != 0; j++) {
            printf("Word %d : Length = %d\n", j + 1, wordLengths[j]);
            printf("Word %d : Vowels = %d\n", j + 1, vowelCounts[j]);
            printf("\n");
        }
*/
        fclose(filePtr);
        int max = find_maximum(wordLengths, wordCounter);

        int lengthCounter[max];
        int vowelCounterOccurrences[max + 1][max];

        countLengths(lengthCounter, (int *) vowelCounterOccurrences, wordLengths, vowelCounts, max, wordCounter);

        printf("Total Number of Words = %d\n", wordCounter);
        printf("Word length\n  ");
        for (int j = 1; j <= max; j++) {
            printf("%10d", j);
        }
        printf("\n  ");
        for (int j = 0; j < max; j++) {
            printf("%10d", lengthCounter[j]);
        }
        printf("\n  ");
        for (int j = 0; j < max; j++) {
            printf("%10.2f", 100 * ((float) lengthCounter[j] / ((float) wordCounter)));
        }
        printf("\n");
        for (int i = 0; i < max + 1; ++i) {
            for (int j = 0; j < max; j++) {
                float result = 100 * ((float) vowelCounterOccurrences[i][j] / ((float) lengthCounter[j]));
                if (j == 0) {
                    printf("%2d", i);
                }
                if (isnanf(result)) {
                    result = 0.0f;
                }
                if (j + 1 >= i) {
                    printf("%10.2f", result);
                } else {
                    printf("%10s", " ");
                }
            }
            printf("\n");
        }
        return 1;
    }
}

bool isntSeparator(unsigned int c, bool thirdByte) {
    unsigned char whitespace = 0x20,
            tab = 0x9,
            period = 0x2e,
            comma = 0x2c,
            hyphen = 0x2d,
            exclPoint = 0x21,
            colon = 0x3a,
            semicolon = 0x3b,
            questMark = 0x3f,
            newline = 0xa,
            carriageReturn = 0x0d,
            leftSquareBracket = 0x5b,
            rightSquareBracket = 0x5d,
            leftParenthesis = 0x28,
            rightParenthesis = 0x29,
            doubleQuotation = 0x22,
            dQuotationLeft = 0x9c,
            dQuotationRight = 0x9d;

    if (thirdByte) {
        if (c == dQuotationLeft || c == dQuotationRight) return false;
    } else {
        if (c == whitespace) return false;
        if (c == tab) return false;
        if (c == period) return false;
        if (c == comma) return false;
        if (c == hyphen) return false;
        if (c == exclPoint) return false;
        if (c == colon) return false;
        if (c == semicolon) return false;
        if (c == questMark) return false;
        if (c == newline) return false;
        if (c == carriageReturn) return false;
        if (c == leftSquareBracket) return false;
        if (c == rightSquareBracket) return false;
        if (c == leftParenthesis) return false;
        if (c == rightParenthesis) return false;
        if (c == doubleQuotation) return false;
    }

    return true;
}

bool isThreeBytePunctuation(unsigned int c) {
    unsigned char dash = 0x93, ellipsis = 0xa6;

    if (c == dash || c == ellipsis) return true;
    else return false;
}

bool isApostropheOrSingleQuote(unsigned int c, bool thirdByte) {
    unsigned char apostrophe = 0x27, sQuotationLeft = 0x98, sQuotationRight = 0x99;

    if (((c == sQuotationLeft || c == sQuotationRight) && thirdByte) || c == apostrophe) return true;
    else return false;
}

bool isAlphaNumericOrUnderscore(unsigned int c) {
    unsigned char zero = 0x30, nine = 0x39, aUpper = 0x41, zUpper = 0x5a,
            aLower = 0x61, zLower = 0x7a, underscore = 0x5f;

    if ((c >= zero && c <= nine) || (c >= aUpper && c <= zUpper) ||
        (c >= aLower && c <= zLower) || c == underscore) {
        return true;
    } else {
        return false;
    }
}

int find_maximum(const int countArray[], int numWords) {
    int c, max;

    max = countArray[0];

    for (c = 1; c < numWords; c++) {
        if (countArray[c] > max) {
            max = countArray[c];
        }
    }

    return max;
}

void countLengths(int *lengthArray, int *vowelFrequencies, int const *wordLengths, int const *vowelCounts,
        int maxLength, int numWords) {
    int length, vowels;
    for (int i = 0; i < (maxLength + 1) * maxLength; i++) {
        if (i < maxLength) {
            lengthArray[i] = 0;
        }
        *((int *) vowelFrequencies + i) = 0;
    }
    for (int i = 0; i < numWords; i++) {
        length = wordLengths[i];
        vowels = vowelCounts[i];
        *(((int *) vowelFrequencies + vowels * maxLength) + length - 1) += 1;
        lengthArray[length - 1] += 1;
    }
}

#endif //CLE_WORDLENGTHS_READER_H
