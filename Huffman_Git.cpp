//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int C_max_len = 100;

const int C_max_char = 256;  // 256 = 2 ** 8

//=============================================================================

struct node_t
{
    char letter;
    node_t *left;
    node_t *right;
};

//=============================================================================

int Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters);

int Find_Frequency (int *arr_of_frequency, const char *text);

void Print (int *arr_of_frequency);

//-----------------------------------------------------------------------------

void Qsort (int *arr, int left, int right, unsigned char *letters);

void My_Swap (int *arr, const int left, const int right, unsigned char *letters);

int Sort_For_Part (int *arr, int left, int right, int mid, unsigned char *letters);

//-----------------------------------------------------------------------------

node_t *Create_Node (const char letter);

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters);

void Sort_After_Union (int *arr_of_frequncy, const int index);

//=============================================================================

int main ()
{
    int *arr_of_frequency = (int *) calloc (C_max_char, sizeof (int));
    unsigned char *letters = (unsigned char *) calloc (C_max_char, sizeof (unsigned char));
    int cnt_of_all_letters = Initialization_And_Make_Arrays (arr_of_frequency, letters);

    Qsort (arr_of_frequency, 0, C_max_char - 1, letters);

    Make_Tree (arr_of_frequency, letters, cnt_of_all_letters);

    return 0;
}

//=============================================================================

int Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters)
{
    char *text = (char *) calloc (C_max_len, sizeof (char));
    scanf ("%s", text);

    for (int i = 0; i < C_max_char; i++) letters[i] = i;

    return Find_Frequency (arr_of_frequency, text);
}

//=============================================================================

int Find_Frequency (int *arr_of_frequency, const char *text)
{
    int cnt_of_all_letters = 0;

    for (int i = 0; i < strlen (text); i++)
    {
        arr_of_frequency[text[i]]++;
        cnt_of_all_letters++;
//        printf ("text[%0.2d] = %c\n", i, text[i]);
    }

    return cnt_of_all_letters;
}

//=============================================================================

void Print (int *arr_of_frequency)
{
    for (int i = 0; i < C_max_char; i++)
    {
        if (arr_of_frequency[i] != 0)
        {
            printf ("%d ", arr_of_frequency[i]);
        }
    }

    printf ("\n");
    printf ("========================================\n");
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
    int helper1 = arr[left];
    arr[left]   = arr[right];
    arr[right]  = helper1;

    unsigned char helper2 = letters[left];
    letters[left]  = letters[right];
    letters[right] = helper2;
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

                My_Swap (arr, left, right, letters);
            }
        }
    }

    return res;
}

//=============================================================================

node_t *Create_Node (const char letter)
{
    node_t* node = (node_t *) calloc (1, sizeof (node_t) + 2);

    node->letter = letter;
    node->left  = nullptr;
    node->right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters)
{
    node_t *node1 = Create_Node (arr_of_frequncy[0]);

    for (int i = 1; i < C_max_char; i++)
    {
        node_t *node2 = Create_Node ('\0');

        node2->left = node1;
        node2->right = Create_Node (arr_of_frequncy[i]);

        node1 = node2;

        arr_of_frequncy[i] += arr_of_frequncy[i - 1];
        arr_of_frequncy[i - 1] = 0;  // for beautiful dump (not necessarily)

        Sort_After_Union (arr_of_frequncy, i);

//        Print (arr_of_frequncy);
    }

    return node;
}

//=============================================================================

void Sort_After_Union (int *arr_of_frequncy, const int index)
{
    int val = arr_of_frequncy[index];

    for (int i = index + 1; i < C_max_char; i++)
    {
        if (val > arr_of_frequncy[i])
        {
            int helper = arr_of_frequncy[i - 1];
            arr_of_frequncy[i - 1] = arr_of_frequncy[i];
            arr_of_frequncy[i] = helper;
        }
        else return;
    }
}
