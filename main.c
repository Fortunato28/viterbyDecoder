#include <stdio.h>
#include <viterby.h>
#include <stdint.h>

int main()
{
    uint32_t data = 0xB;
    uint32_t *coded = &data;
    size_t lenCoded = 20;
    uint32_t *decoded;
    size_t lenDecoded = 4;

    getDecode(coded, lenCoded, decoded, lenDecoded);
    return 0;
}
