/*
 * testEncoding.c
 *
 *  Created on: 13/02/2020
 *      Author: helder
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "testEnconding.h"
#include "reader.h"

int main(int argc, char *argv[]) {
    char *type = test("é");
    printf("Encoding Type: %s\n", type);
    int result;
    for (int i = 1; i < argc; i++) {
        if (access( argv[i], F_OK ) != -1) {
            printf("\nFile name: %s\n", argv[i]);
            result = reader(argv[i], type);
            if (!result) {
                return EXIT_FAILURE;
            }
        } else {
            printf("%s\n", "Invalid argument. Must be a file.");
            return EXIT_FAILURE;
        }
    }

    return 0;
}
