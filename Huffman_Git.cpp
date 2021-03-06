//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "TxLib.h"

#include <string>
#include <iostream>

//=============================================================================

const int C_max_len  = 100;

const int C_max_char = 256;  // 256 = 2 ** 8

//=============================================================================

using ::std::string;

//=============================================================================

struct node_t
{
    char data;
    int freq;  // frequncy
    node_t *left;
    node_t *right;
};

//=============================================================================

const char *Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters);

void Find_Frequency (int *arr_of_frequency, const char *text);

void Print (int *arr_of_frequency);

//-----------------------------------------------------------------------------

void Dump_Dot (node_t* node);

void Dump_Dot_Labels (node_t* node, FILE *fout);

void Dump_Dot_Links (node_t* node, FILE *fout);

//-----------------------------------------------------------------------------

void Qsort (int *arr, int left, int right, unsigned char *letters);

void My_Swap (int *arr, const int left, const int right, unsigned char *letters);

int Sort_For_Part (int *arr, int left, int right, int mid, unsigned char *letters);

//-----------------------------------------------------------------------------

node_t *Create_Node (const char letters, const int frequncy);

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters);

void Insertion_Sort (node_t **helper_arr_of_nodes, unsigned char *letters, const int index);

//-----------------------------------------------------------------------------

void Make_Codes (node_t *node, string *arr_of_codes, string way);

//=============================================================================

void Make_Reverse_Arr (string *arr_of_codes, string *reverse_arr_of_codes);

void Encode (string *arr_of_codes, const char *text);

void Write_Bit (int bit, FILE *fout, bool if_end = false);

unsigned long long int Reverse_Buffer (unsigned long long int buffer, const int len_of_buffer);

//=============================================================================

int main ()
{
    int *arr_of_frequency = (int *) calloc (C_max_char, sizeof (int)); assert (arr_of_frequency);
    unsigned char *letters = (unsigned char *) calloc (C_max_char, sizeof (unsigned char)); assert (letters);
    const char *text = Initialization_And_Make_Arrays (arr_of_frequency, letters);

    Qsort (arr_of_frequency, 0, C_max_char - 1, letters);

    node_t *node = Make_Tree (arr_of_frequency, letters); assert (node);
    Dump_Dot (node);

    string arr_of_codes[C_max_char];
	for (int i = 0; i < C_max_char; i++) arr_of_codes[i] = "";

    Make_Codes (node, arr_of_codes, "");

        for (int i = 0; i < C_max_char; i++)
        {
            if (arr_of_codes[i] != "")
            {
                printf ("(%c) = ", i);
                std::cout << arr_of_codes[i];
                printf ("\n");
            }
        }

//    string reverse_arr_of_codes[C_max_char];
//    Make_Reverse_Arr (arr_of_codes, reverse_arr_of_codes);

//    for (int i = 0; i < C_max_char; i++)
//    {
//        if (arr_of_codes[i] != "")
//        {
//            std::cout << arr_of_codes[i] << "  ---  ";
//            std::cout << reverse_arr_of_codes[i] << "\n";
//        }
//    }

//    Encode (reverse_arr_of_codes, text);
    Encode (arr_of_codes, text);

    free (arr_of_frequency); arr_of_frequency = nullptr;
    free (letters);          letters          = nullptr;

    return 0;
}

//=============================================================================

const char *Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters)
{
    char *text = (char *) calloc (C_max_len, sizeof (char)); assert (text);
    scanf ("%s", text);

    for (int i = 0; i < C_max_char; i++) letters[i] = i;

    Find_Frequency (arr_of_frequency, text);

    return text;
}

//=============================================================================

void Find_Frequency (int *arr_of_frequency, const char *text)
{
    const int len_of_text = strlen (text);

    for (int i = 0; i < len_of_text; i++)
    {
        arr_of_frequency[text[i]]++;
        printf ("text[%0.2d] = %c\n", i, text[i]);
    }
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

void Print (node_t **arr)
{
    for (int i = 0; i < C_max_char; i++)
    {
        if (arr[i]->freq != 0)
        {
            printf ("%d ", arr[i]->freq);
        }
    }

    printf ("\n");
    printf ("========================================\n");
}

//=============================================================================

void Dump_Dot (node_t* node)
{
    FILE *fout = fopen ("DOT", "w"); assert (fout);

    fprintf (fout, "digraph\n");
    fprintf (fout, "{\n");
    fprintf (fout, "\"%p\" [label=\"%c\"]\n", node, node->data);

    assert (node);
    Dump_Dot_Labels (node, fout);

    assert (node);
    Dump_Dot_Links  (node, fout);

    fprintf (fout, "}");

    fclose (fout);
}

//-----------------------------------------------------------------------------

void Dump_Dot_Labels (node_t* node, FILE *fout)
{
    assert (node);
    if (node->left)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->left, (node->left)->data);

        Dump_Dot_Labels (node->left, fout);
    }

    assert (node);
    if (node->right)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->right, (node->right)->data);

        Dump_Dot_Labels (node->right, fout);
    }
}

//-----------------------------------------------------------------------------

void Dump_Dot_Links (node_t* node, FILE *fout)
{
    assert (node);
    if (node->left)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->left);
        Dump_Dot_Links (node->left, fout);
    }

    assert (node);
    if (node->right)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->right);
        Dump_Dot_Links (node->right, fout);
    }
}

//=============================================================================

void Qsort (int *arr, int left, int right, unsigned char *letters)
{
    assert (arr);
    assert (letters);

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
    assert (arr);
    assert (letters);

    int helper1 = arr[left];
    arr[left]   = arr[right];
    arr[right]  = helper1;

    unsigned char helper2 = letters[left];
    letters[left]  = letters[right];
    letters[right] = helper2;
}

void My_Swap (node_t **node1, node_t **node2)
{
    assert (node1);
    assert (node2);

    node_t *helper1 = *node1;
    *node1 = *node2;
    *node2 = helper1;
}

void My_Swap (unsigned char *elem1, unsigned char *elem2)
{
    assert (elem1);
    assert (elem2);

    char helper2 = *elem1;
    *elem1 = *elem2;
    *elem2 = helper2;
}

//=============================================================================

int Sort_For_Part (int *arr, int left, int right, int mid, unsigned char *letters)
{
    assert (letters);

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

node_t *Create_Node (const char letter, const int frequncy)
{
    node_t* node = (node_t *) calloc (1, sizeof (node_t) + 2); assert (node);

    node->data = letter;
    node->freq = frequncy;
    node->left  = nullptr;
    node->right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters)
{
    assert (arr_of_frequncy);
    assert (letters);

    int start = -1;
    while (arr_of_frequncy[++start] == 0);

    node_t **helper_arr_of_nodes = (node_t **) calloc (C_max_char, sizeof (helper_arr_of_nodes[0])); assert (helper_arr_of_nodes);

    for (int i = 0; i < C_max_char; i++)
    {
        helper_arr_of_nodes[i] = Create_Node (letters[i], arr_of_frequncy[i]);
    }

//    for (int i = 0; i < C_max_char; i++)
//    {Encode
//        printf ("%d %d\n", arr_of_frequncy[i], helper_arr_of_nodes[i]->freq);
//    }

    for (int i = start + 1; i < C_max_char; i++)
    {
        node_t *node = Create_Node ('_', helper_arr_of_nodes[i]->freq + helper_arr_of_nodes[i - 1]->freq);
        node->left  = helper_arr_of_nodes[i - 1];
        node->right = helper_arr_of_nodes[i];

        helper_arr_of_nodes[i] = node;

        Insertion_Sort (helper_arr_of_nodes, letters, i);
    }

    return helper_arr_of_nodes[C_max_char - 1];
}

//=============================================================================

void Insertion_Sort (node_t **helper_arr_of_nodes, unsigned char *letters, const int index)
{
    assert (helper_arr_of_nodes);
    assert (letters);

    int val = helper_arr_of_nodes[index]->freq;

    for (int i = index + 1; i < C_max_char; i++)
    {
        if (val > helper_arr_of_nodes[i]->freq)
        {
            My_Swap (&helper_arr_of_nodes[i - 1], &helper_arr_of_nodes[i]);
//            node_t *helper1 = helper_arr_of_nodes[i - 1];
//            helper_arr_of_nodes[i - 1] = helper_arr_of_nodes[i];
//            helper_arr_of_nodes[i] = helper1;

            My_Swap (&letters[i - 1], &letters[i]);
//            char helper2 = letters[i - 1];
//            letters[i - 1] = letters[i];
//            letters[i] = helper2;
        }
    }
}

//=============================================================================

void Make_Codes (node_t *node, string *arr_of_codes, string way)
{
    assert (arr_of_codes);

    assert (node);
    if (node->left != nullptr)
    {
    	way += '0';

    	Make_Codes (node->left, arr_of_codes, way);

    	way.pop_back();
    }

    assert (node);
	if (node->right != nullptr)
    {
    	way += '1';

    	Make_Codes (node->right, arr_of_codes, way);

    	way.pop_back();
    }

    arr_of_codes[node->data] = way;
}

//=============================================================================

void Make_Reverse_Arr (string *arr_of_codes, string *reverse_arr_of_codes)
{
    for (int i = 0; i < C_max_char; i++)
    {
        reverse_arr_of_codes[i] = "";

        string helper = arr_of_codes[i];

        while (helper != "")
        {
            reverse_arr_of_codes[i].push_back (helper.back ());

            helper.pop_back ();
        }
    }
}

//-----------------------------------------------------------------------------

//void Encode (string *reverse_arr_of_codes, const char *text)
void Encode (string *arr_of_codes, const char *text)
{
    FILE *fout = fopen ("output.txt", "w"); assert (fout);

    const int len_of_text = strlen (text);
    for (int i = 0; i < len_of_text; i++)
    {
        assert (arr_of_codes);
        string helper = arr_of_codes[text[i]];

        int len_of_str = helper.size ();
        for (int k = 0; k < len_of_str; k++)
        {
            Write_Bit ((int) helper[k], fout);
        }
    }

    Write_Bit (0, fout, 1);

    fclose (fout);
}

//-----------------------------------------------------------------------------
//                                BIT STREAM                                  ;
//-----------------------------------------------------------------------------

void Write_Bit (int bit, FILE *fout, bool if_end)
{
    if (if_end != 0)
    {
        // We have to do smth with end (len_of_text % 64 != 0)
    }

    static int cnt = 0;
    static unsigned long long int buffer = 0;

    buffer = (buffer << 1) | (bit & 1);
    cnt++;

    if (cnt == 64)
    {
//        for (int i = 0; i < cnt ; i++)
//        {
//            printf ("%d", (buffer >> i) & 1);
//        }
//        printf ("\n----------------------\n");

        unsigned long long int rev_buffer = Reverse_Buffer (buffer, cnt);

//        for (int i = 0; i < cnt ; i++)
//        {
//            printf ("%d", (rev_buffer >> i) & 1);
//        }

        assert (fout);
        fprintf (fout, "llu", rev_buffer); // we write number or we have to write string?

        cnt = 0;
        buffer = 0;
    }
}

//-----------------------------------------------------------------------------

unsigned long long int Reverse_Buffer (unsigned long long int buffer, const int len_of_buffer)
{
    unsigned long long int res = 0;

    for (int i = 0; i < len_of_buffer; i++)
    {
        res = (res << 1) | ((buffer >> i) & 1);
    }

    return res;
}
