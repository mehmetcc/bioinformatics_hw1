#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/**
 * TODOs
 * Add comparison metrics
 * Define time metrics with microseconds
**/

/** function prototypes */
// functions return -1 if there is no match
int naive_search(char *input, char *pattern);
int kmp(char *input, char *pattern);
void preprocess(char *pattern, int *lps, int M);
int boyer_moore(char *input, char *pattern);

int main()
{
    /** Declarations for time benchmarking */
    struct timeval start, end;
    unsigned long start_in_ms, end_in_ms;

    /** Define the input and the pattern in here */
    char *input = "TAAGTCTATACCATCGTAGTCTAATTAACGTTATGGTAGGATATCAAGGACGGAATGACCGCAGAGGCGACGTTAATGCGCCGTCAGAGACGCCCTAAAGATTGCGGTAGGGTCCCGTTGTTAAAGAGACTTGAGTGGGTGCTTGATGGGAGTGTATTAAGGGCATGTATAAGTGTTGCTGGGTCTAAGGCATTAAAGCTGAGTCAATAGTTACATTGCAGATTAACGAGATCTGAAATTAAGGGAGAGATTCCCAGAGTGGCCTAGTACTTAAGGGCACCCACGCCGCAGGCGGCCCTACGCCCGTTAATGGTTCGAGTGCTATTCACTAACACATTAACGGACGTTTAGTGTGGATTATAGGTGAAGGGTCTGCGCCACTCCAAGGCAGGGAACATATGTGTTGTTACTATCTTAACG";
    char *pattern = "TGGGTCTAAGGCATTAAAGCTGAGTCAATAGT";

    /** run Naive algorithm */
    gettimeofday(&start, NULL);
    int found = naive_search(input, pattern);
    gettimeofday(&end, NULL);
    // print results for Naive algorithm
    printf("Running Naive algorithm\n");
    printf("Found at index %d\n", found);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);

    /** run KMP algorithm */
    gettimeofday(&start, NULL);
    found = kmp(input, pattern);
    gettimeofday(&end, NULL);
    // print results for KMP algorithm
    printf("Running KMP algorithm\n");
    printf("Found at index %d\n", found);
    printf("Start time: %d\nEnd time: %d\n", start.tv_usec, end.tv_usec);
    printf("Total time elapsed: %d microseconds\n", end.tv_usec - start.tv_usec);
}

/** functions */

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
                break;

        if (j == M)
        {
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
        if (pattern[j] == input[i])
        {
            j++;
            i++;
        }
        // process patterns
        if (j == M)
        {
            return i - j;
        }
        else if (i < N && pattern[j] != input[i])
        {
            if (j != 0)
            {
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
        if (pattern[i] == pattern[suffix_length])
        {
            suffix_length++;
            lps[i] = suffix_length;
            i++;
        }
        else
        {
            if (suffix_length != 0)
            {
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

    return -1;
}