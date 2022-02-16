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
#define MAX_CHAR_LEN 256
const unsigned long  MAX_BUF_LEN = 1 << 30;
#define MAX(x, y) (x) > (y) ? (x) : (y)

int debugPrint(int* info, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%d  ", info[i]);
    }
    printf("\n");
    return 0;
}

int calcBC(string subdir, int bc[])
{
    int i = 0;
    int len = subdir.size();

    for (i = 0; i < MAX_CHAR_LEN; i++)
    {
        bc[i] = len;
    }
    for (i = 0; i < len - 1; i++)
    {
        bc[subdir[i]] = len - 1 - i;
    }

    return 0;
}

int calcSuffix(string subdir, int suffix[])
{
    int i = 0;
    int j = 0;
    int k = 0;
    int count = 0;
    int len = subdir.size();

    suffix[len - 1] = len;
    for (i = len - 2; i >= 0; i--)
    {
        j = i;
        k = len - 1;
        count = 0;
        while ((subdir[j--] == subdir[k--])&&(j>-1&&k>-1))
        {
            count++;
        }
        suffix[i] = count;
    }

    return 0;
}

int calcGS(string subdir, int gs[])
{
    int i = 0;
    int j = 0;
    int len = subdir.size();

    int* suffix = (int*)malloc(sizeof(int) * len);
    if (!suffix)
    {
        printf("malloc error!\n");
        return 0;
    }

    calcSuffix(subdir, suffix);

    //debugPrint(suffix,len);
    for (i = 0; i < len; i++)
    {
        gs[i] = len;
    }

    j = 0;
    for (i = len - 2; i >= 0; --i)
    {
        if (suffix[i] == i + 1)
        {
            for (; j < len - 1; j++)
            {
                if (gs[j] == len)
                {
                    gs[j] = len - 1 - i;
                }
            }
        }
    }

    for (i = 0; i <= len - 2; i++)
    {
        gs[len - 1 - suffix[i]] = len - 1 - i;
    }

    if (suffix)
    {
        free(suffix);
        suffix = NULL;
    }

    return 0;
}

int BoyerMoore(string szText, int nTextLen, string szSubstr, int nStrLen)
{
    //int textlen = strlen(text);
    //int sublen = strlen(subdir);
    int i = 0;
    int j = 0;

    int* bc = (int*)malloc(sizeof(int) * MAX_CHAR_LEN);
    int* gs = (int*)malloc(sizeof(int) * nStrLen);

    calcBC(szSubstr, bc);
    calcGS(szSubstr, gs);

    //printf("%s\n",szText);
    //printf("%s\n",szSubstr);

    //debugPrint(gs, nStrLen);
    //searching
    while (j <= nTextLen - nStrLen)
    {
        for (i = nStrLen - 1; i >= 0 && szSubstr[i] == szText[j + i]; i--);

        //find
        if (i < 0)
        {
            printf("The Pattern was found at the position:%d\n", j);
            //j += gs[0];
            //break;
            return 0;
        }
        else
        {
            //j += bc[szText[j + i]] - nStrLen + 1 + i;
            j += MAX(bc[szText[j + i]] - nStrLen + 1 + i, gs[i]);
        }
    }

    printf("not foud str:%s\n", szSubstr);
    return 0;
}

string readFileIntoString2(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
            << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int main(void)
{

    //string g_szText = "acagtatggcaacccttcctcctattaggagaccgcgcttagtataacttgcactgtttg ccccgtgtcaatgtaaattcgagtaacctcttagagccgacaccttgcgacatactcagg gaatcatcctacttgccagcgttcgataccctgccgattatactgggacccaccgcgttc ctgggctcgccttatctcccgtgatgggctttccgactatgggagcggaactctataaat ggcggcactcagagtgagccattcaattcgtggtctgggagtcagtgaacacttgctgtc gcatagcgttagagaactcacgatatttggtcggccagttgcggttgttgatctgggtca gcgaggagcaggtctgcgccattgttgcgggtcctccttccataacgcttgctcgatccc agagagctaaacgcaaagcccgagctcttgaaggcgtattactgtccgaggcagtggcgc tcacctgcctgttccgtgtcgcagatttcattatgtctagtaaaaaaggattagttgagc tgagccgcctttgaagagaacgattctgcgctctattacgtcgttccagagaaattaccg cgggatctctaggggaacgagacgatcaccattccgcctttaggcatatatattatacgt cctccgccaattgtgaggtgcaaaatagttgcaatcaccgccaagtactttcgctaagcc ttgcgcctatacagctctcgccgtacggtgccctcgatatgcaagtcgaaccctcccact ttagtgatgatcattgctatgtattggtaccaggcggcgacgagcgagagacaatgcttt acttgaggcacgccatcttgtaagggactgtcgccctttgtagcaatagggaaagttcct aagtagttcgtgactttggcttgcaacggcgtacgatgatagccgatcgtgtaagcgaca gatagcatgaaagtacgccgtatactagaagtacacccaataacgaactacgtcagaaac aggcggggattgcatgtaatgtcctgagctacaatgctggctgcaagccacatgtcccac ttattcttgaaatcaggtgtcagttatatgcatctggggtgccataactgtatcgggcct gcgtgattcttcgtattctgcgcacagcaacatcacctcttcggcgattttaatgttcga ttggttaatccgtaattcgatagggtcgctgctggaatgggcaatcgtcttgcttacaat cccgtgggtaaagccgtgcggcgagcgagttgcgaagttcaacttcccgacaaccgaaac ccccgacaaggtagcgtatttgctcattatggcaaccaatttgacttagtatggttgggt gactccaatttttccggcccagcaatcggccaaccttcttcaaccatcgacccgaagcga cccccggaccagacccgtgtcgtgcatgatcccgaacatcgtagaatccctggcgtaagc tgttcgtgtattatcggaaagaaacaaacactcgcgtgcgtggaatagcgaagcgaccga tcatgaccctgctgtcggctgtagagtcctctagtatagacaatgtacacctgtatcagg cagagacttattcggccctgatcacaagaaaaaaaacgttcacgggccggggggatcagc caatcatgcctgtgaatcgactctcgatcgtctaccgtaaggacaaccgcactcacgctg ctagttcgtttgaacagttaacgtcctacccagcgaagactgggtctaggaagtactcgt aaattgtagtagttgcgattggagggatagcgaaagtgggcgggtttaagttggcagctg tatgcttcaagtacctctcggtactacagtctggagtgtgggcaacacgagcttctgtac caggtaagcaaatgtacgtggtctagggaaatctaacaaggaaggccacgcgatcaataa aagcagcaggacctatagaccagttctttagccgatattggatgccaggactctctggac ggcgagatcctcccggtggctacacaagtaatcgtaagaaatcaaatcgccgtacgtacc acaacgcataccgcatgataaaaaaaacgagacgtaaattgtattcaacgcctcatgctc cgttagagtagggcacggagaacagaggccgcccgccaccgcagatcctaagttggaccg atagcgaatcatagaacttttggctgggactccgtaatggttagcacccatgcgctctgc gcgacctgcattcccctttatgccagctaaccagacggccccatagtgtccctcggtagt ttggtgaagccgtatctttggagaaacctaaaccctgttgtgcctccgctatttttccgc cggtatacgcaataggatgtccaacgaattcttaaactaaggggcttcggatcaggcggg gccggctgcggcggatgtaggtgcctagtgaacccaccgtagcccgtacaattcgattag cctgagtcggaagccggcgtagaaaaaactacgcgatgtaccatcgcctggggttcagag aatttttctcgtcgatcttttatgccgcgtccccacacggggccttagttatccgtccca gcgtctgggcaaagaggcgagacatttagcacgagccgcaatccctctagccttcggatt aatgacgctgttaggtcagcgacactaaaaccgaaggaatggcctgggtcaagacatact aaagtgtcagtaactcgcaaatgagggttgtcccccgctcacttatagttgaaaggctcg ccgactaggccgatgttttcatgtataagaagcagcgagaatcttcaatgcgtatcaacc gagaaactttagcaattcgccttaggggtggcgctgcggaccgatgtaagacaactatag gcacgagcat";
    //string szStr = "accg atagcgaatcatag";
    string filename("Sample.txt");
    //string file_contents;

    string g_szText = readFileIntoString2(filename);
    string szStr;// = "agt tactcgtgcgtattacct";
    START:std::getline(std::cin, szStr);
    //begin time
    auto start = std::chrono::steady_clock::now();
    //search
    BoyerMoore(g_szText, MAX_BUF_LEN, szStr, szStr.size());
    //end time
    cout << "\nPattern : " << szStr;
    auto end = std::chrono::steady_clock::now();
    cout << "\n\nElapsed time in micro seconds: "
        << chrono::duration_cast<chrono::microseconds>(end - start).count()
        << " micro seconds\n";
    goto START;
    return 0;
}