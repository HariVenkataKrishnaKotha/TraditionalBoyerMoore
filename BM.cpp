#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<string>
#include<sstream>
#include<fstream>
#include <iostream>
#include<chrono>
using namespace std;
#define Maximum_Character_Length 256
const unsigned long  Maximum_Buffer_Length = 1 << 30;
#define MAX(x, y) (x) > (y) ? (x) : (y)

int CalculateBadCharacter(string Pattern, int bad_character[])
{
    int i = 0;
    int len = Pattern.size();

    for (i = 0; i < Maximum_Character_Length; i++)
    {
        bad_character[i] = len;
    }
    for (i = 0; i < len - 1; i++)
    {
        bad_character[Pattern[i]] = len - 1 - i;
    }

    return 0;
}

int CalculateSuffix(string Pattern, int suffix[])
{
    int i = 0;
    int j = 0;
    int k = 0;
    int count = 0;
    int len = Pattern.size();

    suffix[len - 1] = len;
    for (i = len - 2; i >= 0; i--)
    {
        j = i;
        k = len - 1;
        count = 0;
        while ((Pattern[j--] == Pattern[k--])&&(j>-1&&k>-1))
        {
            count++;
        }
        suffix[i] = count;
    }

    return 0;
}

int CalculateGoodSuffix(string Pattern, int good_suffix[])
{
    int i = 0;
    int j = 0;
    int len = Pattern.size();

    int* suffix = (int*)malloc(sizeof(int) * len);
    if (!suffix)
    {
        printf("malloc error!\n");
        return 0;
    }

    CalculateSuffix(Pattern, suffix);

    //debugPrint(suffix,len);
    for (i = 0; i < len; i++)
    {
        good_suffix[i] = len;
    }

    j = 0;
    for (i = len - 2; i >= 0; --i)
    {
        if (suffix[i] == i + 1)
        {
            for (; j < len - 1; j++)
            {
                if (good_suffix[j] == len)
                {
                    good_suffix[j] = len - 1 - i;
                }
            }
        }
    }

    for (i = 0; i <= len - 2; i++)
    {
        good_suffix[len - 1 - suffix[i]] = len - 1 - i;
    }

    if (suffix)
    {
        free(suffix);
        suffix = NULL;
    }

    return 0;
}

int BoyerMoore(string Text, int TxtLen, string Pattern, int PatLen)
{
    //int textlen = strlen(text);
    //int sublen = strlen(Pattern);
    int i = 0;
    int j = 0;

    int* bad_character = (int*)malloc(sizeof(int) * Maximum_Character_Length);
    int* good_suffix = (int*)malloc(sizeof(int) * PatLen);

    CalculateBadCharacter(Pattern, bad_character);
    CalculateGoodSuffix(Pattern, good_suffix);

    //printf("%s\n",Text);
    //printf("%s\n",Pattern);

    //debugPrint(good_suffix, PatLen);
    //searching
    while (j <= TxtLen - PatLen)
    {
        for (i = PatLen - 1; i >= 0 && Pattern[i] == Text[j + i]; i--);

        //find
        if (i < 0)
        {
            printf("The Pattern was found at the position:%d\n", j);
            //j += good_suffix[0];
            //break;
            return 0;
        }
        else
        {
            //j += bad_character[Text[j + i]] - PatLen + 1 + i;
            j += MAX(bad_character[Text[j + i]] - PatLen + 1 + i, good_suffix[i]);
        }
    }

    printf("Pattern not found:%s\n", Pattern);
    return 0;
}

string FiletoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Please check the file - '"
            << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int main(void)
{

    //string Text = "acagtatggcaacccttcctcctattaggagaccgcgcttagtataacttgcactgtttg ccccgtgtcaatgtaaattcgagtaacctcttagagccgacaccttgcgacatactcagg gaatcatcctacttgccagcgttcgataccctgccgattatactgggacccaccgcgttc ctgggctcgccttatctcccgtgatgggctttccgactatgggagcggaactctataaat ggcggcactcagagtgagccattcaattcgtggtctgggagtcagtgaacacttgctgtc gcatagcgttagagaactcacgatatttggtcggccagttgcggttgttgatctgggtca gcgaggagcaggtctgcgccattgttgcgggtcctccttccataacgcttgctcgatccc agagagctaaacgcaaagcccgagctcttgaaggcgtattactgtccgaggcagtggcgc tcacctgcctgttccgtgtcgcagatttcattatgtctagtaaaaaaggattagttgagc tgagccgcctttgaagagaacgattctgcgctctattacgtcgttccagagaaattaccg cgggatctctaggggaacgagacgatcaccattccgcctttaggcatatatattatacgt cctccgccaattgtgaggtgcaaaatagttgcaatcaccgccaagtactttcgctaagcc ttgcgcctatacagctctcgccgtacggtgccctcgatatgcaagtcgaaccctcccact ttagtgatgatcattgctatgtattggtaccaggcggcgacgagcgagagacaatgcttt acttgaggcacgccatcttgtaagggactgtcgccctttgtagcaatagggaaagttcct aagtagttcgtgactttggcttgcaacggcgtacgatgatagccgatcgtgtaagcgaca gatagcatgaaagtacgccgtatactagaagtacacccaataacgaactacgtcagaaac aggcggggattgcatgtaatgtcctgagctacaatgctggctgcaagccacatgtcccac ttattcttgaaatcaggtgtcagttatatgcatctggggtgccataactgtatcgggcct gcgtgattcttcgtattctgcgcacagcaacatcacctcttcggcgattttaatgttcga ttggttaatccgtaattcgatagggtcgctgctggaatgggcaatcgtcttgcttacaat cccgtgggtaaagccgtgcggcgagcgagttgcgaagttcaacttcccgacaaccgaaac ccccgacaaggtagcgtatttgctcattatggcaaccaatttgacttagtatggttgggt gactccaatttttccggcccagcaatcggccaaccttcttcaaccatcgacccgaagcga cccccggaccagacccgtgtcgtgcatgatcccgaacatcgtagaatccctggcgtaagc tgttcgtgtattatcggaaagaaacaaacactcgcgtgcgtggaatagcgaagcgaccga tcatgaccctgctgtcggctgtagagtcctctagtatagacaatgtacacctgtatcagg cagagacttattcggccctgatcacaagaaaaaaaacgttcacgggccggggggatcagc caatcatgcctgtgaatcgactctcgatcgtctaccgtaaggacaaccgcactcacgctg ctagttcgtttgaacagttaacgtcctacccagcgaagactgggtctaggaagtactcgt aaattgtagtagttgcgattggagggatagcgaaagtgggcgggtttaagttggcagctg tatgcttcaagtacctctcggtactacagtctggagtgtgggcaacacgagcttctgtac caggtaagcaaatgtacgtggtctagggaaatctaacaaggaaggccacgcgatcaataa aagcagcaggacctatagaccagttctttagccgatattggatgccaggactctctggac ggcgagatcctcccggtggctacacaagtaatcgtaagaaatcaaatcgccgtacgtacc acaacgcataccgcatgataaaaaaaacgagacgtaaattgtattcaacgcctcatgctc cgttagagtagggcacggagaacagaggccgcccgccaccgcagatcctaagttggaccg atagcgaatcatagaacttttggctgggactccgtaatggttagcacccatgcgctctgc gcgacctgcattcccctttatgccagctaaccagacggccccatagtgtccctcggtagt ttggtgaagccgtatctttggagaaacctaaaccctgttgtgcctccgctatttttccgc cggtatacgcaataggatgtccaacgaattcttaaactaaggggcttcggatcaggcggg gccggctgcggcggatgtaggtgcctagtgaacccaccgtagcccgtacaattcgattag cctgagtcggaagccggcgtagaaaaaactacgcgatgtaccatcgcctggggttcagag aatttttctcgtcgatcttttatgccgcgtccccacacggggccttagttatccgtccca gcgtctgggcaaagaggcgagacatttagcacgagccgcaatccctctagccttcggatt aatgacgctgttaggtcagcgacactaaaaccgaaggaatggcctgggtcaagacatact aaagtgtcagtaactcgcaaatgagggttgtcccccgctcacttatagttgaaaggctcg ccgactaggccgatgttttcatgtataagaagcagcgagaatcttcaatgcgtatcaacc gagaaactttagcaattcgccttaggggtggcgctgcggaccgatgtaagacaactatag gcacgagcat";
    //string Pattern = "accg atagcgaatcatag";
    string filename("Sample.txt");
    //string file_contents;

    string Text = FiletoString(filename);
    string Pattern;// = "agt tactcgtgcgtattacct";
    START:std::getline(std::cin, Pattern);
    //begin time
    auto start = std::chrono::steady_clock::now();
    //search
    BoyerMoore(Text, Maximum_Buffer_Length, Pattern, Pattern.size());
    //end time
    cout << "\nPattern : " << Pattern;
    auto end = std::chrono::steady_clock::now();
    cout << "\n\nElapsed time in micro seconds: "
        << chrono::duration_cast<chrono::microseconds>(end - start).count()
        << " micro seconds\n";
    goto START;
    return 0;
}