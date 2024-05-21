#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE_ARR 300000
#define INF_VAL_MAX   2000000000
#define INF_VAL_MIN   -2000000000
#define MAX_SIZE_COM   20
#define MAX(x, y)   \
    x < y ? y : x

struct
{
  int value;
} typedef Elem_heap;

void swap(Elem_heap *l1, Elem_heap *l2)
{
  Elem_heap x = *l1;
  *l1 = *l2;
  *l2 = x;
}

struct
{
  int size;
  Elem_heap *arr_heap;
} typedef BinHeap;

int sift_up_min(BinHeap *heap, int index);
int sift_up_max(BinHeap *heap, int index);

void sift_down_min(BinHeap *heap, int index);
void sift_down_max(BinHeap *heap, int index);

Elem_heap *get_ptr(BinHeap *heap, int index)
{
  return heap->arr_heap + index;
}

int cmp_elem_heap(Elem_heap *fst, Elem_heap *scd)
{
  return fst->value - scd->value;
}

enum TYPE_LAYERS
{
  MIN_LAYER = 1,
  MAX_LAYER = 0
};

enum TYPE_LAYERS def_type_layer(int index)
{
  int amount  = 1;
  int counter = 0;
  
  while(index >= (amount - 1))
  {
    amount *= 2;
    counter++;
  }
  if(counter % 2 == 1)    return MIN_LAYER;
  return MAX_LAYER;
}


void sift_up(BinHeap *heap, int index)
{
  if(index >= 1)
  {
    enum TYPE_LAYERS type = def_type_layer(index);

    int parent = (index - 1) / 2;
    if(type == MIN_LAYER)
    {
      if(cmp_elem_heap(get_ptr(heap, index), get_ptr(heap, parent)) > 0)
      {
        swap(get_ptr(heap, index), get_ptr(heap, parent));
        sift_up_max(heap, parent);
      }
      else
      {
        sift_up_min(heap, index);
      }
    }
    else
    {
      if(cmp_elem_heap(get_ptr(heap, index), get_ptr(heap, parent)) < 0)
      {
        swap(get_ptr(heap, index), get_ptr(heap, parent));
        sift_up_min(heap, parent);
      }
      else
      {
        sift_up_max(heap, index);
      }
    }
  }
}

int sift_up_min(BinHeap *heap, int index)
{
  while(index >= 3)
  {
    int parent = (index - 3) / 4;

    if(cmp_elem_heap(get_ptr(heap, index), get_ptr(heap, parent)) < 0)
    {
      swap(get_ptr(heap, index), get_ptr(heap, parent));
      index = parent;
    }
    else
    {
      break;
    }
  }
  return 1;
}

int sift_up_max(BinHeap *heap, int index)
{
  while(index >= 6)
  {
    int parent = (index - 3) / 4;

    if(cmp_elem_heap(get_ptr(heap, index), get_ptr(heap, parent)) > 0)
    {
      swap(get_ptr(heap, index), get_ptr(heap, parent));
      index = parent;
    }
    else
    {
      break;
    }
  }
  return 1;
}

void sift_down(BinHeap *heap, int index)
{
  enum TYPE_LAYERS type = def_type_layer(index);

  (type == MIN_LAYER) ? (sift_down_min(heap, index)) : (sift_down_max(heap, index));
}

void sift_down_min(BinHeap *heap, int index)
{
  int parent = (index - 1) / 2;
  if((cmp_elem_heap(get_ptr(heap, parent), get_ptr(heap, index)) < 0) && (index != 0) && (get_ptr(heap, index)->value != INF_VAL_MAX))
  {
      swap(get_ptr(heap, parent), get_ptr(heap, index));
      sift_down_min(heap, index); //
      return;
  }

  int child1 = 4 * index + 3;
  int child2 = 4 * index + 4;
  int child3 = 4 * index + 5;
  int child4 = 4 * index + 6;

  int child5 = 2 * index + 1;
  int child6 = 2 * index + 2;
  int ind_min      = index  ;

  bool is_son = false;

  if(child1 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child1), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child1;
    }
  }
  if(child2 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child2), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child2;
    }
  }
  if(child3 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child3), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child3;
    }
  }
  if(child4 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child4), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child4;
    }
  }
  if(child5 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child5), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child5;
      is_son = true;
    }
  }

  if(child6 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child6), get_ptr(heap, ind_min)) < 0))
    {
      ind_min = child6;
      is_son = true;
    }
  }

  if(ind_min != index)
  {
    swap(get_ptr(heap, index), get_ptr(heap, ind_min));
    (is_son) ? (sift_down_max(heap, ind_min)) : (sift_down_min(heap, ind_min));
  }
}

void sift_down_max(BinHeap *heap, int index)
{
  int parent = (index - 1) / 2;
  
  if((cmp_elem_heap(get_ptr(heap, parent), get_ptr(heap, index)) > 0) && (index != 0) && (get_ptr(heap, index)->value != INF_VAL_MIN))
  {
    swap(get_ptr(heap, parent), get_ptr(heap, index));
    sift_down_max(heap, index);
    return;
  }

  int child1 = 4 * index + 3;
  int child2 = 4 * index + 4;
  int child3 = 4 * index + 5;
  int child4 = 4 * index + 6;

  int child5 = 2 * index + 1;
  int child6 = 2 * index + 2;

  bool is_son = false;
  int ind_min = index ;

  if(child1 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child1), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child1;
    }
  }
  if(child2 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child2), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child2;
    }
  }
  if(child3 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child3), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child3;
    }
  }
  if(child4 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child4), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child4;
    }
  }
  if(child5 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child5), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child5;
      is_son = true;
    }
  }

  if(child6 < heap->size)
  {
    if((cmp_elem_heap(get_ptr(heap, child6), get_ptr(heap, ind_min)) > 0))
    {
      ind_min = child6;
      is_son = true;
    }
  }

  if(ind_min != index)
  {
    swap(get_ptr(heap, index), get_ptr(heap, ind_min));
    (is_son) : (sift_down_min(heap, ind_min)) : (sift_down_max(heap, ind_min))ж
  }
}

void insert(BinHeap *heap, int val_to_ins)
{
  get_ptr(heap, heap->size)->value = val_to_ins;
  sift_up(heap, heap->size);

  heap->size++;
}

int getMin(BinHeap *heap)
{
  return (heap->size == 0) ? INF_VAL_MAX : (get_ptr(heap, 0))->value;
}

int getMax(BinHeap *heap)
{
  int index_max = 0;
  if(heap->size == 0) return INF_VAL_MAX;
  else if(heap->size == 1) index_max = 0;
  else if(heap->size == 2) index_max = 1;
  else
  {
    index_max = (get_ptr(heap, 1)->value < get_ptr(heap, 2)->value ? 2 : 1);
  }
  int val_for_ret = heap->arr_heap[index_max].value;
  return val_for_ret;
}

int extractMin(BinHeap *heap)
{
  if(heap->size == 0) return 0;

  enum TYPE_LAYERS type = def_type_layer(heap->size - 1);
  int val_for_ret = heap->arr_heap[0].value;

  (type == MIN_LAYER) ? ((get_ptr(heap, 0)->value) = INF_VAL_MAX) : ((get_ptr(heap, 0)->value) = INF_VAL_MIN);
  swap(get_ptr(heap, 0), get_ptr(heap, heap->size - 1));
  heap->size--;
  sift_down(heap, 0);
  return val_for_ret;
}

int extractMax(BinHeap *heap)
{
  if(heap->size == 0) return 0;
  int index_max = 0;

  enum TYPE_LAYERS type = def_type_layer(heap->size - 1);
  if(heap->size == 1) index_max = 0;
  else if(heap->size == 2) index_max = 1;
  else
  {
    index_max = (get_ptr(heap, 1)->value < get_ptr(heap, 2)->value ? 2 : 1);
  }
  int val_for_ret = heap->arr_heap[index_max].value;

  (type == MIN_LAYER) ? ((get_ptr(heap, index_max)->value) = INF_VAL_MAX) : ((get_ptr(heap, index_max)->value) = INF_VAL_MIN);
  swap(get_ptr(heap, index_max), get_ptr(heap, heap->size - 1));
  heap->size--;
  sift_down(heap, index_max);

  return val_for_ret;
}

void init_BinHeap(BinHeap *heap, int size)
{
  int amount = 1;
  int counter = 0;
  heap->arr = (Elem_heap *)calloc(size, sizeof(Elem_heap));
  for(int i = 0; i < size; i++)
  {
    if(i >= (amount - 1))
    {
      amount *= 2;
      counter++;
    }
    (counter % 2 == 1) ? ((get_ptr(heap, i)->value) = INF_VAL_MAX) : ((get_ptr(heap, i)->value) = INF_VAL_MIN);
  }
  heap->size = 0;
}
int main()
{
  int amount_op = 0, index = 0;
  long int val = 0;
  char command[MAX_SIZE_COM] = {};
  scanf("%d", &amount_op);

  BinHeap heap;
    
  init_BinHeap(&heap, MAX_SIZE_ARR);

  for(int i = 0; i < amount_op; i++)
  {
    scanf("%*s", MAX_SIZE_COM, command);

    if(strcmp(command, "insert") == 0)
    {
      scanf("%d", &val);
      insert(&heap, val);
      printf("ok\n");
    }
    else if(strcmp(command, "get_min") == 0)
    {
      val = getMin(&heap);
      (val == INF_VAL_MAX) ? (printf("error\n")) : (printf("%d\n", val));
    }

    else if(strcmp(command, "extract_min") == 0)
    {
      int res = extractMin(&heap);
      (res == 0) ? (printf("error\n")) : (printf("%d\n", res));
    }
    else if(strcmp(command, "get_max") == 0)
    {
      val = getMax(&heap);
      (val == INF_VAL_MAX) ? (printf("error\n")) : (printf("%d\n", val));
    }
    else if(strcmp(command, "extract_max") == 0)
    {
      int res = extractMax(&heap);
      (res == 0) ? (printf("error\n")) : (printf("%d\n", res));
    }
    else if(strcmp(command, "clear") == 0)
    {
      init_BinHeap(&heap, heap.size);
      printf("ok\n");
    }
    else if(strcmp(command, "size") == 0)
    {
      printf("%d\n", heap.size);
    }
  }
free(heap->arr);
}
