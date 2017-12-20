#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_N 8

typedef uint8_t func[MAX_N];

static inline void num_to_func(uint8_t n, uint64_t nf, func *f)
{
    for (unsigned i = 0; i < n; i++)
    {
        (*f)[i] = nf % n;
        nf /= n;
    }
}

static inline unsigned func_comp(unsigned n, const func *f, const func *g)
{
    for (unsigned i = 0; i < n; i++)
    {
        if ((*f)[(*g)[(*f)[i]]] != (*g)[(*g)[(*f)[i]]]) return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    unsigned N;
    uint64_t card = 1;
    uint64_t count = 0;

    if (argc != 2)
    {
        printf("%s <N>\n", argv[0]);
        exit(0);
    }

    N = strtoul(argv[1], NULL, 0);

    for (unsigned i = 0; i < N; i++)
    {
        card *= N;
    }

#pragma omp parallel for reduction(+:count)
    for (uint64_t fnum = 0; fnum < card; fnum++) 
    {
        func f;
        num_to_func(N, fnum, &f);
        for (uint64_t gnum = 0; gnum < card; gnum++) 
        {
            func g;
            num_to_func(N, gnum, &g);
            count += func_comp(N, &f, &g);
        }
    }

    printf("%u: %lu\n", N, count);

    return 0;
}
