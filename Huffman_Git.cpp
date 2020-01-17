//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int C_max_len = 100;

char *Find_Frequency (const char *text);

//=============================================================================

int main ()
{
    char *text = (char *) calloc (C_max_len, sizeof (char));

    scanf ("%s", text);

    char *arr_of_frequency = Find_Frequency (text);

    for (int i = 0; i < 256; i++)
    {
        printf ("[%03d]{%c} = %d", i, i, arr_of_frequency[i]);

        if (arr_of_frequency[i] != 0)
        {
            printf ("  NOT ZERO!!!");
        }

        printf ("\n");
    }

    return 0;
}

//=============================================================================

char *Find_Frequency (const char *text)
{
    char *res = (char *) calloc (256, sizeof (char));  // 256 = 2 ** 8

    for (int i = 0; i < strlen (text); i++)
    {
        res[text[i]]++;

        printf ("text[%d] = %c\n", i, text[i]);
    }

    return res;
}
