#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE_ARR 2000000
#define INF_VAL_MAX  100000000000000000
#define INF_VAL_MIN  -2000000000
#define MAX_SIZE_COM 20

struct
{
    long long int value;
    int           numb_op;
}typedef Elem_heap;

struct
{
    Elem_heap *ptr_elem_heap[MAX_SIZE_ARR];
    int counter_op;
    int size;
    Elem_heap arr_heap[MAX_SIZE_ARR];
}typedef BinHeap;

void swap(Elem_heap *element_1, Elem_heap *element_2)
{
    Elem_heap temp = *element_1;
    *element_1 = *element_2;
    *element_2 = temp;
}

void swap_ptr(Elem_heap **element_1, Elem_heap **element_2)
{
    Elem_heap *temp = *element_1;
    *element_1 = *element_2;
    *element_2 = temp;
}

Elem_heap *get_ptr(BinHeap *heap, int index)
{
    return heap->arr_heap + index;
}

long long int cmp_elem_heap(Elem_heap *fst, Elem_heap *scd)
{
    return fst->value - scd->value;
}

void sift_up(BinHeap *heap, int index)
{
    while(index != 0)
    {
        int parent = (index - 1) / 2;

        if(cmp_elem_heap(get_ptr(heap, index), get_ptr(heap, parent)) < 0)
        {
            swap(get_ptr(heap, index), get_ptr(heap, parent));
            swap_ptr(heap->ptr_elem_heap + get_ptr(heap, index)->numb_op, heap->ptr_elem_heap + get_ptr(heap, parent)->numb_op);

            index = parent;
        }
        else
        {
            break;
        }
    }
}

void sift_down(BinHeap *heap, int index)
{
    int first_child  = 2 * index + 1;
    int second_child = 2 * index + 2;
    int ind_min      = index        ;

    if(first_child < heap->size)
    {
        if((cmp_elem_heap(get_ptr(heap, first_child), get_ptr(heap, ind_min)) < 0))
        {
            ind_min = first_child;
        }
    }
    if(second_child < heap->size)
    {
        if((cmp_elem_heap(get_ptr(heap, second_child), get_ptr(heap, ind_min)) < 0))
        {
            ind_min = second_child;
        }
    }

    if(ind_min != index)
    {
        swap(get_ptr(heap, index), get_ptr(heap, ind_min));
        swap_ptr(heap->ptr_elem_heap + get_ptr(heap, index)->numb_op, heap->ptr_elem_heap + get_ptr(heap, ind_min)->numb_op);
        sift_down(heap, ind_min);
    }

}

void insert(BinHeap *heap, long long int val_to_ins)
{
    heap->counter_op++;

    get_ptr(heap, heap->size)->value = val_to_ins;
    get_ptr(heap, heap->size)->numb_op = heap->counter_op;
    heap->ptr_elem_heap[heap->counter_op] = get_ptr(heap, heap->size);
    sift_up(heap, heap->size);

    heap->size++;
}

long long int getMin(BinHeap *heap)
{
    heap->counter_op++;

    return (get_ptr(heap, 0))->value;
}

void extractMin(BinHeap *heap)
{
    (get_ptr(heap, 0)->value) = INF_VAL_MAX;
    swap(get_ptr(heap, 0), get_ptr(heap, heap->size - 1));
    swap_ptr(heap->ptr_elem_heap + get_ptr(heap, 0)->numb_op, heap->ptr_elem_heap + get_ptr(heap, heap->size - 1)->numb_op);
    sift_down(heap, 0);
    heap->size--;
    heap->counter_op++;
}

void init_BinHeap(BinHeap *heap)
{
    for(int i = 0; i < MAX_SIZE_ARR; i++)
    {
        get_ptr(heap, i)->value = INF_VAL_MAX;
        heap->ptr_elem_heap[i] = NULL;
    }
    heap->size = 0;
    heap->counter_op = 0;
}

int main()
{
    int amount_op = 0, index = 0;
    long int val = 0;
    char command[MAX_SIZE_COM] = {};
    scanf("%d", &amount_op);

    BinHeap heap;
    init_BinHeap(&heap);

    for(int i = 0; i < amount_op; i++)
    {
        scanf("%s", command);

        if(strcmp(command, "insert") == 0)
        {
            scanf("%lld", &val);
            insert(&heap, val);
        }
        else if(strcmp(command, "decreaseKey") == 0)
        {
            scanf("%d %lld", &index, &val);
            (heap.ptr_elem_heap[index])->value -= val;
            if((val > 0) && ((heap.ptr_elem_heap[index] - heap.arr_heap) != 0))
            {
                sift_up(&heap, (heap.ptr_elem_heap[index] - heap.arr_heap));
            }
            else
            {
                sift_down(&heap, (heap.ptr_elem_heap[index] - heap.arr_heap));
            }
            heap.counter_op++;
        }
        else if(strcmp(command, "getMin") == 0)
        {
            val = getMin(&heap);
            printf("%lld\n", val);
        }
        else if(strcmp(command, "extractMin") == 0)
        {
            extractMin(&heap);
        }
    }
}
