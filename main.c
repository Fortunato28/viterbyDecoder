#include <stdio.h>
#include <viterby.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
    uint32_t data = 0xD7677;
    uint32_t *coded = &data;
    size_t lenCoded = 20;
    size_t lenDecoded = lenCoded / N - 6;
    uint32_t *decoded = (uint32_t *)malloc(lenDecoded);	// Формула представлена в пункте 7.3 G.9903


    //printf("Bytes: %d\n", lenDecoded / 8 + 1);
    getDecode(coded, lenCoded, decoded, lenDecoded);

    free(decoded);
    return 0;
}
