//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int C_max_len = 100;

//=============================================================================

struct node_t
{
    char *data;
    node_t *left;
    node_t *right;
};

//=============================================================================

int *Find_Frequency (const char *text);

void Qsort (int *arr, int left, int right, unsigned char *letters);

void My_Swap (int *arr, const int left, const int right, unsigned char *letters);

int Sort_For_Part (int *arr, int left, int right, int mid, unsigned char *letters);

//=============================================================================

int main ()
{
    char *text = (char *) calloc (C_max_len, sizeof (char));

    scanf ("%s", text);

    int *arr_of_frequency = Find_Frequency (text);

    unsigned char *letters = (unsigned char *) calloc (256, sizeof (unsigned char));
    for (int i = 0; i < 256; i++) letters[i] = i;

    Qsort (arr_of_frequency, 0, 255, letters);

    for (int i = 0; i < 256; i++)
    {
        printf ("[%03d]{%c} = %d", i, letters[i], arr_of_frequency[i]);

        if (arr_of_frequency[i] != 0)
        {
            printf ("  NOT ZERO!!!");
        }

        printf ("\n");
    }

    return 0;
}

//=============================================================================

int *Find_Frequency (const char *text)
{
    int *res = (int *) calloc (256, sizeof (int));  // 256 = 2 ** 8

    for (int i = 0; i < strlen (text); i++)
    {
        res[text[i]]++;

        printf ("text[%0.2d] = %c\n", i, text[i]);
    }

    return res;
}

//=============================================================================

void Qsort (int *arr, int left, int right, unsigned char *letters)
{
    if (right - left + 1 == 2)
    {
        if (arr[left] > arr[right])
        {
            My_Swap (arr, left, right, letters);
        }
    }
    else if (left >= right)
    {
        return ;
    }
    else
    {
		int mid = Sort_For_Part (arr, left, right, (left + right) / 2, letters);

        Qsort (arr, left, mid - 1,  letters);
        Qsort (arr, mid + 1, right, letters);
    }
}

//=============================================================================

void My_Swap (int *arr, const int left, const int right, unsigned char *letters)
{
//    printf ("LEFT = [%d], RIGHT = [%d]\n", left, right);
//    printf ("1:Arr[left] = %d, Arr[right] = %d\n\n", arr[left], arr[right]);
    int helper1 = arr[left];
    arr[left]   = arr[right];
    arr[right]  = helper1;

    unsigned char helper2 = letters[left];
    letters[left]  = letters[right];
    letters[right] = helper2;

//    printf ("2:Arr[left] = %d, Arr[right] = %d\n", arr[left], arr[right]);
}

//=============================================================================

int Sort_For_Part (int *arr, int left, int right, int mid, unsigned char *letters)
{
    int res = mid;

    while (left < right)
    {
        while (arr[left] < arr[res] && left < right)
        {
            left++;
        }

        while (arr[right] > arr[res] && left < right)
        {
            right--;
        }

        if (left < right)
        {
            if (arr[left] == arr[right])
            {
                if (right == res)
                    left++;
                else if (left == res)
                    right --;
                else
                {
                    left++;
                    right--;
                }
            }
            else
            {
                if (arr[left] == arr[res])
                    res = right;
                else if (arr[right] == arr[res])
                    res = left;

//                printf ("1:Arr[left] = %d, Arr[right] = %d\n", arr[left], arr[right]);
                My_Swap (arr, left, right, letters);
//                printf ("2:Arr[left] = %d, Arr[right] = %d\n", arr[left], arr[right]);
            }
        }
    }

    return res;
}
