#include <iostream>
#include <array>

#define MAX_N 8

typedef std::array<uint8_t, MAX_N> func;

static inline void num_to_func(uint8_t n, uint64_t nf, func *f)
{
    for (unsigned i = 0; i < n; i++)
    {
        (*f)[i] = nf % n;
        nf /= n;
    }
}

static inline unsigned func_comp(unsigned n, const func &f, const func &g)
{
    for (unsigned i = 0; i < n; i++)
    {
        if (f[g[f[i]]] != g[g[f[i]]]) return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    unsigned N;
    uint64_t card = 1;
    unsigned count = 0;

    if (argc != 2)
    {
        std::cout << argv[0] << " <N>" << std::endl;
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
            count += func_comp(N, f, g);
        }
    }

    std::cout << N << ": " << count << std::endl;

    return 0;
}
