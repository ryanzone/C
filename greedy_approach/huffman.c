/**
 * Huffman coding implementation in C.
 * Huffman Coding is a lossless data compression algorithm. It assigns
 * variable-length codes to input characters, with shorter codes assigned to
 * more frequent characters. This algorithm makes sure that the most common
 * characters are represented by shorter bit strings, reducing the overall size
 * of the encoded data.
 *
 * @author GeeksforGeeks (https://www.geeksforgeeks.org/c/huffman-coding-in-c/)
 *
 * functions used in this implementation:
 *
 * - newNode(): creates a new node for the Huffman tree.
 * - push(): adds a node to the min-heap.
 * - pop(): removes and returns the node with the smallest frequency from the
 *          min-heap.
 * - buildCodes(): generates the Huffman codes for each character by traversing
 *                 the Huffman tree.
 *
 * My notes from my DAA class
 * @author Ryan John Mathew: https://github.com/ryanzone
 * the diagram representation of the tree with
 * sample structure of the tree with steps:
 *
 * 1. [5] [9] [12] [13] [16] [45]
 *     \   /
 *      [14] [12] [16] [45]
 *
 *
 *
 * 2. [12] [13]  [14]  [16]  [45]
 *                / \
 *      \   /   [5] [9]
 *
 *       [25]  [14]  [16] [45]
 *            / \
 *           [5] [9]
 *
 *
 *
 * 3. [25]       [30]  [45]
 *    \  /       /   \
 *  [12] [13]   [14] [16]
 *              \  /
 *              [5] [9]
 *
 *
 *
 * 4. [45]      [55]
 *              /  \
 *          [25]    [30]
 *          /  \    /  \
 *       [12] [13] [14] [16]
 *                  / \
 *                 [5] [9]
 *
 *
 *
 * 5.               [100]
 *                /    \
 *             [45]     [55]
 *                       /  \
 *                  [25]    [30]
 *                  /  \    /  \
 *              [12] [13]  [14] [16]
 *                        / \
 *                      [5] [9]
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 256

typedef struct Node
{
    char ch;
    int freq;
    struct Node *l, *r;
} Node;

Node* heap[MAX];
int hs = 0;

Node* newNode(char ch, int freq, Node* l, Node* r)
{
    Node* n = malloc(sizeof(Node));
    *n = (Node){ch, freq, l, r};
    return n;
}

void push(Node* n)
{
    int i = hs++;
    while (i > 0 && n->freq < heap[(i - 1) / 2]->freq)
    {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i] = n;
}

Node* pop()
{
    Node* min = heap[0];
    Node* last = heap[--hs];
    int i = 0, c;
    while ((c = 2 * i + 1) < hs)
    {
        if (c + 1 < hs && heap[c + 1]->freq < heap[c]->freq)
            c++;
        if (last->freq <= heap[c]->freq)
            break;
        heap[i] = heap[c];
        i = c;
    }
    heap[i] = last;
    return min;
}

char codes[MAX][MAX];
int clen[MAX];

void buildCodes(Node* n, int* code, int d)
{
    if (!n->l && !n->r)
    {
        for (int i = 0; i < d; i++)
            codes[(unsigned char)n->ch][i] = '0' + code[i];
        codes[(unsigned char)n->ch][d] = '\0';
        clen[(unsigned char)n->ch] = d;
        return;
    }
    code[d] = 0;
    buildCodes(n->l, code, d + 1);
    code[d] = 1;
    buildCodes(n->r, code, d + 1);
}

int main()
{
    srand(time(NULL));
    int n = rand() % 5 + 4;
    char chars[8];
    int freq[8], used[26] = {};

    for (int i = 0; i < n; i++)
    {
        int r;
        do
        {
            r = rand() % 26;
        } while (used[r]);
        used[r] = 1;
        chars[i] = 'a' + r;
        freq[i] = rand() % 50 + 1;
    }

    printf("Input:\n");
    for (int i = 0; i < n; i++) printf("  '%c' = %d\n", chars[i], freq[i]);

    for (int i = 0; i < n; i++) push(newNode(chars[i], freq[i], NULL, NULL));
    while (hs > 1)
    {
        Node *l = pop(), *r = pop();
        push(newNode('$', l->freq + r->freq, l, r));
    }

    Node* root = pop();
    int code[MAX];
    buildCodes(root, code, 0);

    printf("\nChar | Freq | Code         | Bits\n");
    printf("-----|------|--------------|-----\n");
    int total = 0, tbits = 0;
    for (int i = 0; i < n; i++)
    {
        int b = freq[i] * clen[(unsigned char)chars[i]];
        tbits += b;
        total += freq[i];
        printf(" '%c' |  %-3d | %-12s | %d\n", chars[i], freq[i],
               codes[(unsigned char)chars[i]], b);
    }

    int fb = total * (int)ceil(log2(n));
    printf("\nTotal chars   : %d\n", total);
    printf("Huffman bits  : %d\n", tbits);
    printf("Fixed bits    : %d (%d-bit)\n", fb, (int)ceil(log2(n)));
    printf("Space saved   : %.1f%%\n", 100.0 * (fb - tbits) / fb);
}
