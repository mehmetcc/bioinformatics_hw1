#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#define LARGE 256

int rabin_karp(char *input, char *pattern, int heuristic, int large)
{
    int M = strlen(pattern);
    int N = strlen(input);
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * large) % heuristic;

    for (i = 0; i < M; i++)
    {
        p = (large * p + pattern[i]) % heuristic;
        t = (large * t + input[i]) % heuristic;
    }

    for (i = 0; i <= N - M; i++)
    {

        if (p == t)
        {
            for (j = 0; j < M; j++)
            {
                if (input[i + j] != pattern[j])
                    break;
            }

            if (j == M)
                return i;
        }

        if (i < N - M)
        {
            t = (large * (t - input[i] * h) + input[i + M]) % heuristic;

            if (t < 0)
                t = (t + heuristic);
        }
    }

    return -1;
}

int main() {
    char *input = "TAAGTCTATACCATCGTAGTCTAATTAACGTTATGGTAGGATATCAAGGACGGAATGACCGCAGAGGCGACGTTAATGCGCCGTCAGAGACGCCCTAAAGATTGCGGTAGGGTCCCGTTGTTAAAGAGACTTGAGTGGGTGCTTGATGGGAGTGTATTAAGGGCATGTATAAGTGTTGCTGGGTCTAAGGCATTAAAGCTGAGTCAATAGTTACATTGCAGATTAACGAGATCTGAAATTAAGGGAGAGATTCCCAGAGTGGCCTAGTACTTAAGGGCACCCACGCCGCAGGCGGCCCTACGCCCGTTAATGGTTCGAGTGCTATTCACTAACACATTAACGGACGTTTAGTGTGGATTATAGGTGAAGGGTCTGCGCCACTCCAAGGCAGGGAACATATGTGTTGTTACTATCTTAACG";
    char *pattern = "TGGGTCTAAGGCATTAAAGCTGAGTCAATAGT";

    int found = rabin_karp(input, pattern, 97, 256);
    printf("found at %d\n", found);
}