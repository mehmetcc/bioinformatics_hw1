#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>


/**
 * TODOs
 * Add command line interface for file i/o
 * Add comparison metrics (how much comparison will be made to find the pattern) ---> done
 * Define time metrics with microseconds
**/

/** macros */
#define LARGE 256
#define R_K_HEURISTIC 77
#define BUZZ_SIZE 256

// define a max(a,b) macro
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

/** global variables */
int comparison = 0;

/** function prototypes */
// functions return -1 if there is no match
char *read_from_file(char *filename);
int naive_search(char *input, char *pattern);
int kmp(char *input, char *pattern);
void preprocess(char *pattern, int *lps, int M);
int boyer_moore(char *input, char *pattern);
int encode_array(char *str, int size, int *bad_characters);
int rabin_karp(char *input, char *pattern, int heuristic, int large);

int main(int argc, char *argv[])
{
    /** Declarations for time benchmarking */
    struct timeval start, end;
    unsigned long start_in_ms, end_in_ms;

    char *input, *pattern;

    /** define logic for taking command line arguments */
    if (argc >= 3)
    {
        input = read_from_file(argv[1]);
        pattern = read_from_file(argv[2]);

        printf("%s\n", pattern); // testing
    }
    else
    {
        input = "TAAGTCTATACCATCGTAGTCTAATTAACGTTATGGTAGGATATCAAGGACGGAATGACCGCAGAGGCGACGTTAATGCGCCGTCAGAGACGCCCTAAAGATTGCGGTAGGGTCCCGTTGTTAAAGAGACTTGAGTGGGTGCTTGATGGGAGTGTATTAAGGGCATGTATAAGTGTTGCTGGGTCTAAGGCATTAAAGCTGAGTCAATAGTTACATTGCAGATTAACGAGATCTGAAATTAAGGGAGAGATTCCCAGAGTGGCCTAGTACTTAAGGGCACCCACGCCGCAGGCGGCCCTACGCCCGTTAATGGTTCGAGTGCTATTCACTAACACATTAACGGACGTTTAGTGTGGATTATAGGTGAAGGGTCTGCGCCACTCCAAGGCAGGGAACATATGTGTTGTTACTATCTTAACG";
        pattern = "TGGGTCTAAGGCATTAAAGCTGAGTCAATAGT";
    }

    /** run Naive algorithm */
    gettimeofday(&start, NULL);
    int found = naive_search(input, pattern);
    gettimeofday(&end, NULL);
    // print results for Naive algorithm
    printf("Running Naive algorithm\n");
    printf("Found at index %d\n", found + 1);
    printf("Total comparisons: %d\n", comparison);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);

    /** run KMP algorithm */
    comparison = 0;
    gettimeofday(&start, NULL);
    found = kmp(input, pattern);
    gettimeofday(&end, NULL);
    // print results for KMP algorithm
    printf("Running KMP algorithm\n");
    printf("Found at index %d\n", found + 1);
    printf("Total comparisons: %d\n", comparison);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);

    /** run Rabin-Karp algorithm */
    comparison = 0;
    gettimeofday(&start, NULL);
    found = rabin_karp(input, pattern, R_K_HEURISTIC, LARGE);
    gettimeofday(&end, NULL);
    // print results for Rabin-Karp algorithm
    printf("Running Rabin-Karp algorithm\n");
    printf("Found at index %d\n", found + 1);
    printf("Total comparisons: %d\n", comparison);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);

    /** run Boyer Moore algorithm */
    comparison = 0;
    gettimeofday(&start, NULL);
    found = boyer_moore(input, pattern);
    gettimeofday(&end, NULL);
    // print results for Boyer Moore algorithm
    printf("Running Boyer-Moore algorithm\n");
    printf("Found at index %d\n", found + 1);
    printf("Total comparisons: %d\n", comparison);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);
}

/** functions */
char *read_from_file(char *filename) {
    return NULL;
}
// naive search implementation
int naive_search(char *input, char *pattern)
{
    int M = strlen(pattern);
    int N = strlen(input);

    for (int i = 0; i <= N - M; i++)
    {

        int j;

        for (j = 0; j < M; j++)
            if (input[i + j] != pattern[j])
            {
                comparison++;
                break;
            }

        if (j == M)
        {
            comparison++;
            return i;
        }
    }
    return -1;
}

// kmp search implementation
int kmp(char *input, char *pattern)
{
    int N = strlen(input);
    int M = strlen(pattern);

    int lps[M];
    int j = 0;

    preprocess(pattern, lps, M);

    int i = 0;

    while (i < N)
    {
        comparison++;
        if (pattern[j] == input[i])
        {
            comparison++;
            j++;
            i++;
        }
        // process patterns
        if (j == M)
        {
            comparison++;
            return i - j;
        }
        else if (i < N && pattern[j] != input[i])
        {
            comparison++;
            if (j != 0)
            {
                comparison++;
                j = lps[j - 1];
            }
            else
            {
                i = i + 1;
            }
        }
    }

    return -1;
}

void preprocess(char *pattern, int *lps, int M)
{
    int suffix_length = 0;
    int i = 1;
    lps[0] = 0;

    while (i < M)
    {
        comparison++;
        if (pattern[i] == pattern[suffix_length])
        {
            comparison++;
            suffix_length++;
            lps[i] = suffix_length;
            i++;
        }
        else
        {
            if (suffix_length != 0)
            {
                comparison++;
                suffix_length = lps[suffix_length - 1];
            }
            else
            {
                lps[i] = suffix_length;
                i++;
            }
        }
    }
}

// boyer moore search implementation
int boyer_moore(char *input, char *pattern)
{
    int N = strlen(input);
    int M = strlen(pattern);

    int bad_characters[LARGE];
    encode_array(pattern, M, bad_characters);

    int S = 0;

    while (S <= (N - M))
    {
        comparison++;
        int j = M - 1;

        while (j >= 0 && pattern[j] == input[S + j])
        {
            comparison++;
            j--;
        }

        if (j < 0)
        {
            comparison++;
            S += (S + M < N) ? M - bad_characters[input[S + M]] : 1;
            return S;
        }
        else
        {
            int max_value = max(1, j - bad_characters[input[S + j]]);
            S += max_value;
        }
    }
}

int encode_array(char *str, int size, int *bad_characters)
{

    int i;

    for (i = 0; i < LARGE; i++)
        bad_characters[i] = -1;

    for (i = 0; i < size; i++)
        bad_characters[(int)str[i]] = i;
}

/** rabin-karp implementation */
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
            comparison++;
            for (j = 0; j < M; j++)
            {
                if (input[i + j] != pattern[j])
                {
                    comparison++;
                    break;
                }
            }

            if (j == M)
            {
                comparison++;
                return i;
            }
        }

        if (i < N - M)
        {
            comparison++;
            t = (large * (t - input[i] * h) + input[i + M]) % heuristic;

            if (t < 0)
            {
                comparison++;
                t = (t + heuristic);
            }
        }
    }

    return -1;
}