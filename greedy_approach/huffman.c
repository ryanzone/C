/**
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

#include <stdio.h>
#include <stdlib.h>

#define MAX 256

typedef struct Node
{
    char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

typedef struct
{
    unsigned size;
    unsigned capacity;
    Node** array;
} MinHeap;

Node* createNode(char data, unsigned freq)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap(unsigned capacity)
{
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (Node**)malloc(capacity * sizeof(Node*));
    return heap;
}

void swapNode(Node** a, Node** b)
{
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size &&
        heap->array[left]->freq < heap->array[smallest]->freq)
        smallest = left;

    if (right < heap->size &&
        heap->array[right]->freq < heap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

Node* extractMin(MinHeap* heap)
{
    Node* temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return temp;
}
void insertMinHeap(MinHeap* heap, Node* node)
{
    heap->size++;
    int i = heap->size - 1;

    while (i && node->freq < heap->array[(i - 1) / 2]->freq)
    {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->array[i] = node;
}
int isSizeOne(MinHeap* heap) { return (heap->size == 1); }

Node* buildHuffmanTree(char data[], int freq[], int size)
{
    Node *left, *right, *top;

    MinHeap* heap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        heap->array[i] = createNode(data[i], freq[i]);

    heap->size = size;

    for (int i = (heap->size - 2) / 2; i >= 0; --i) minHeapify(heap, i);

    while (!isSizeOne(heap))
    {
        left = extractMin(heap);
        right = extractMin(heap);

        top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(heap, top);
    }

    return extractMin(heap);
}
void printCodes(Node* root, int arr[], int top)
{
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (!root->left && !root->right)
    {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i) printf("%d", arr[i]);
        printf("\n");
    }
}

int main(void)
{
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int freq[] = {5, 9, 12, 13, 16, 45};
    int size = sizeof(arr) / sizeof(arr[0]);

    Node* root = buildHuffmanTree(arr, freq, size);

    int codes[MAX];
    printCodes(root, codes, 0);

    return 0;
}
