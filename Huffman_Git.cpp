//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <iostream>//=============================================================================
//                                 HUFFMAN                                    ;
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <iostream>

const int C_max_len = 100;

const int C_max_char = 256;  // 256 = 2 ** 8

//=============================================================================

struct node_t
{
    char data;
    int freq;  // frequncy
    node_t *left;
    node_t *right;
};

//=============================================================================

int Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters);

int Find_Frequency (int *arr_of_frequency, const char *text);

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

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters);

void Sort_After_Union (node_t **helper_arr_of_nodes, unsigned char *letters, const int index);

//-----------------------------------------------------------------------------

void Make_Codes (node_t *node, std::string *arr_of_codes, std::string way);

//=============================================================================

int main ()
{
    int *arr_of_frequency = (int *) calloc (C_max_char, sizeof (int));
    unsigned char *letters = (unsigned char *) calloc (C_max_char, sizeof (unsigned char));
    int cnt_of_all_letters = Initialization_And_Make_Arrays (arr_of_frequency, letters);

    Qsort (arr_of_frequency, 0, C_max_char - 1, letters);

    node_t *node = Make_Tree (arr_of_frequency, letters, cnt_of_all_letters);
    Dump_Dot (node);

    std::string arr_of_codes[C_max_char];
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
        printf ("text[%0.2d] = %c\n", i, text[i]);
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
    FILE *fout = fopen ("DOT", "w");

    fprintf (fout, "digraph\n");
    fprintf (fout, "{\n");
    fprintf (fout, "\"%p\" [label=\"%c\"]\n", node, node->data);

    Dump_Dot_Labels (node, fout);
    Dump_Dot_Links  (node, fout);

    fprintf (fout, "}");

    fclose (fout);
}

//-----------------------------------------------------------------------------

void Dump_Dot_Labels (node_t* node, FILE *fout)
{
    if (node->left)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->left, (node->left)->data);

        Dump_Dot_Labels (node->left, fout);
    }

    if (node->right)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->right, (node->right)->data);

        Dump_Dot_Labels (node->right, fout);
    }
}

//-----------------------------------------------------------------------------

void Dump_Dot_Links (node_t* node, FILE *fout)
{
    if (node->left)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->left);
        Dump_Dot_Links (node->left, fout);
    }

    if (node->right)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->right);
        Dump_Dot_Links (node->right, fout);
    }
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

node_t *Create_Node (const char letter, const int frequncy)
{
    node_t* node = (node_t *) calloc (1, sizeof (node_t) + 2);

    node->data = letter;
    node->freq = frequncy;
    node->left  = nullptr;
    node->right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters)
{
    int start = -1;
    while (arr_of_frequncy[++start] == 0);

    node_t **helper_arr_of_nodes = (node_t **) calloc (C_max_char, sizeof (helper_arr_of_nodes[0]));
    for (int i = 0; i < C_max_char; i++)
    {
        helper_arr_of_nodes[i] = Create_Node (letters[i], arr_of_frequncy[i]);
    }

//    for (int i = 0; i < C_max_char; i++)
//    {
//        printf ("%d %d\n", arr_of_frequncy[i], helper_arr_of_nodes[i]->freq);
//    }

    for (int i = start + 1; i < C_max_char; i++)
    {
        node_t *node = Create_Node ('_', helper_arr_of_nodes[i]->freq + helper_arr_of_nodes[i - 1]->freq);
        node->left  = helper_arr_of_nodes[i - 1];
        node->right = helper_arr_of_nodes[i];

        helper_arr_of_nodes[i] = node;

        Sort_After_Union (helper_arr_of_nodes, letters, i);
    }

    return helper_arr_of_nodes[C_max_char - 1];
}

//=============================================================================

void Sort_After_Union (node_t **helper_arr_of_nodes, unsigned char *letters, const int index)
{
    printf ("1:\n");
    Print (helper_arr_of_nodes);

    int val = helper_arr_of_nodes[index]->freq;
    printf ("val = [%d]\n", val);

    for (int i = index + 1; i < C_max_char; i++)
    {
        printf ("helper_arr_of_nodes[i]->freq = [%d]\n", helper_arr_of_nodes[i]->freq);
        if (val > helper_arr_of_nodes[i]->freq)
        {
            node_t *helper1 = helper_arr_of_nodes[i - 1];
            helper_arr_of_nodes[i - 1] = helper_arr_of_nodes[i];
            helper_arr_of_nodes[i] = helper1;

            char helper2 = letters[i - 1];
            letters[i - 1] = letters[i];
            letters[i] = helper2;
        }
        else
        {
            printf ("2:\n");
            Print (helper_arr_of_nodes);

            return;
        }
    }

    printf ("2:\n");
    Print (helper_arr_of_nodes);
}

//=============================================================================

void Make_Codes (node_t *node, std::string *arr_of_codes, std::string way)
{
    if (node->left != nullptr)
    {
    	way += '0';

    	Make_Codes (node->left, arr_of_codes, way);

    	way.pop_back();
    }

	if (node->right != nullptr)
    {
    	way += '1';

    	Make_Codes (node->right, arr_of_codes, way);

    	way.pop_back();
    }

    arr_of_codes[node->data] = way;
}


const int C_max_len = 100;

const int C_max_char = 256;  // 256 = 2 ** 8

//=============================================================================

struct node_t
{
    char data;
    int freq;  // frequncy
    node_t *left;
    node_t *right;
};

//=============================================================================

int Initialization_And_Make_Arrays (int *arr_of_frequency, unsigned char *letters);

int Find_Frequency (int *arr_of_frequency, const char *text);

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

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters);

void Sort_After_Union (node_t **helper_arr_of_nodes, unsigned char *letters, const int index);

//-----------------------------------------------------------------------------

void Make_Codes (node_t *node, std::string *arr_of_codes, std::string way);

//=============================================================================

int main ()
{
    int *arr_of_frequency = (int *) calloc (C_max_char, sizeof (int));
    unsigned char *letters = (unsigned char *) calloc (C_max_char, sizeof (unsigned char));
    int cnt_of_all_letters = Initialization_And_Make_Arrays (arr_of_frequency, letters);

    Qsort (arr_of_frequency, 0, C_max_char - 1, letters);

    node_t *node = Make_Tree (arr_of_frequency, letters, cnt_of_all_letters);
    Dump_Dot (node);

    std::string arr_of_codes[C_max_char];
	for (int i = 0; i < C_max_char; i++) arr_of_codes[i] = "";

    Make_Codes (node, arr_of_codes, "");

    printf ("AAAA\n");

    for (int i = 0; i < C_max_char; i++)
    {
        if (arr_of_codes[i] != "")
        {
            printf ("(%c) = \n", i);
            std::cout << arr_of_codes[i];
        }
    }

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
        printf ("text[%0.2d] = %c\n", i, text[i]);
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
    FILE *fout = fopen ("DOT", "w");

    fprintf (fout, "digraph\n");
    fprintf (fout, "{\n");
    fprintf (fout, "\"%p\" [label=\"%c\"]\n", node, node->data);

    Dump_Dot_Labels (node, fout);
    Dump_Dot_Links  (node, fout);

    fprintf (fout, "}");

    fclose (fout);
}

//-----------------------------------------------------------------------------

void Dump_Dot_Labels (node_t* node, FILE *fout)
{
    if (node->left)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->left, (node->left)->data);

        Dump_Dot_Labels (node->left, fout);
    }

    if (node->right)
    {
        fprintf (fout, "\"%p\" [label=\"%c\"]\n", node->right, (node->right)->data);

        Dump_Dot_Labels (node->right, fout);
    }
}

//-----------------------------------------------------------------------------

void Dump_Dot_Links (node_t* node, FILE *fout)
{
    if (node->left)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->left);
        Dump_Dot_Links (node->left, fout);
    }

    if (node->right)
    {
        fprintf (fout, "\"%p\"->\"%p\";\n", node, node->right);
        Dump_Dot_Links (node->right, fout);
    }
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

node_t *Create_Node (const char letter, const int frequncy)
{
    node_t* node = (node_t *) calloc (1, sizeof (node_t) + 2);

    node->data = letter;
    node->freq = frequncy;
    node->left  = nullptr;
    node->right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (int *arr_of_frequncy, unsigned char *letters, const int cnt_of_all_letters)
{
    int start = -1;
    while (arr_of_frequncy[++start] == 0);

    node_t **helper_arr_of_nodes = (node_t **) calloc (C_max_char, sizeof (helper_arr_of_nodes[0]));
    for (int i = 0; i < C_max_char; i++)
    {
        helper_arr_of_nodes[i] = Create_Node (letters[i], arr_of_frequncy[i]);
    }

//    for (int i = 0; i < C_max_char; i++)
//    {
//        printf ("%d %d\n", arr_of_frequncy[i], helper_arr_of_nodes[i]->freq);
//    }

    for (int i = start + 1; i < C_max_char; i++)
    {
        node_t *node = Create_Node ('_', helper_arr_of_nodes[i]->freq + helper_arr_of_nodes[i - 1]->freq);
        node->left  = helper_arr_of_nodes[i - 1];
        node->right = helper_arr_of_nodes[i];

        helper_arr_of_nodes[i] = node;

        Sort_After_Union (helper_arr_of_nodes, letters, i);
    }

    return helper_arr_of_nodes[C_max_char - 1];
}

//=============================================================================

void Sort_After_Union (node_t **helper_arr_of_nodes, unsigned char *letters, const int index)
{
    printf ("1:\n");
    Print (helper_arr_of_nodes);

    int val = helper_arr_of_nodes[index]->freq;
    printf ("val = [%d]\n", val);

    for (int i = index + 1; i < C_max_char; i++)
    {
        printf ("helper_arr_of_nodes[i]->freq = [%d]\n", helper_arr_of_nodes[i]->freq);
        if (val > helper_arr_of_nodes[i]->freq)
        {
            node_t *helper1 = helper_arr_of_nodes[i - 1];
            helper_arr_of_nodes[i - 1] = helper_arr_of_nodes[i];
            helper_arr_of_nodes[i] = helper1;

            char helper2 = letters[i - 1];
            letters[i - 1] = letters[i];
            letters[i] = helper2;
        }
        else
        {
            printf ("2:\n");
            Print (helper_arr_of_nodes);

            return;
        }
    }

    printf ("2:\n");
    Print (helper_arr_of_nodes);
}

//=============================================================================

void Make_Codes (node_t *node, std::string *arr_of_codes, std::string way)
{
    if (node->left != nullptr)
    {
    	way += '0';

    	Make_Codes (node->left, arr_of_codes, way);

    	way.pop_back();
    }

	if (node->right != nullptr)
    {
    	way += '1';

    	Make_Codes (node->right, arr_of_codes, way);

    	way.pop_back();
    }

    arr_of_codes[node->data] = way;
}
