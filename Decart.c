#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define POISON_VAL 2000000
#define MAX(a, b) \
  (a > b ? a : b)
#define DEEP_LIMIT 1000000
#define LOOP_LIMIT 100000

int deep = 0;
extern struct ElemTree;

typedef struct
{
  struct  ElemTree *prev;
  int     val;
  int     priorety;
  struct  ElemTree* right;
  struct  ElemTree* left;
  int     size;
} ElemTree;

typedef struct{
  ElemTree *min;
  ElemTree *max;
}SlicersTree;

void SetHigh1(ElemTree *head)
{
  if(head == NULL)  return;
  ElemTree *left_tree = head->left;
  ElemTree *right_tree = head->right;
  if((right_tree == NULL) && (left_tree == NULL)) head->size = 1;
  else if(left_tree == NULL)                      head->size = right_tree->size + 1;
  else if(right_tree == NULL)                     head->size = left_tree->size + 1;
  else                                            head->size = left_tree->size + right_tree->size + 1;
}

void SetHigh(ElemTree *head)
{
  deep++;
  assert(deep < DEEP_LIMIT);
  if(head == NULL) return;

  SetHigh(head->left);
  SetHigh(head->right);
  SetHigh1(head);
}

ElemTree *GetChild(ElemTree *head, bool right)
{
  if(head == NULL)  return NULL;
  
  return (right) ? head->right : head->left;
}

SlicersTree SliceDecart(ElemTree *head, int val_to_slice)
{
  deep++;
  assert(deep < DEEP_LIMIT);

  SlicersTree to_ret = {};
  if(head == NULL)  return to_ret;
  SlicersTree res;
  if(val_to_slice > head->val)
  {
    res = SliceDecart(head->right, val_to_slice);
    head->right = res.min;
    SetHigh1(head->right);
    SetHigh1(head);
    if(res.min != NULL) res.min->prev = head ;
    to_ret.min = head;
    to_ret.max = res.max;
    SetHigh1(res.max);
  }
  else
  {
    res = SliceDecart(head->left, val_to_slice);
    head->left = res.max;
    SetHigh1(head->left);
    SetHigh1(head);
    if(res.max != NULL) res.max->prev = head ;
    to_ret.min = res.min;
    to_ret.max = head;
    SetHigh1(res.min);
  }
  return to_ret;
}

ElemTree *MergeDecart(ElemTree *min, ElemTree *max)
{
  deep++;
  assert(deep < DEEP_LIMIT);

  if((min == NULL) && (max == NULL))  return NULL;
  else if(min == NULL)                return max;
  else if(max == NULL)                return min;
  ElemTree *head, *another;
  if(min->priorety > max->priorety)
  {
    max->left = MergeDecart(min, max->left);
    SetHigh1(max->left);
    another = max->left;
    if(another != NULL)   another->prev = max;
    SetHigh1(max);
    return max;
  }
  else
  {
    min->right = MergeDecart(min->right, max);
    SetHigh1(min->right);
    another = min->right;
    if(another != NULL)   another->prev = min;
    SetHigh1(min);
    return min;
  }
}

ElemTree *Insert(ElemTree *tree, ElemTree * val)
{
  SlicersTree res = SliceDecart(tree, val->val);
  ElemTree *new_min = MergeDecart(res.min, val);
  tree = MergeDecart(new_min, res.max);
  return tree;
}

ElemTree *Delete(ElemTree *tree, int val)
{
  SlicersTree res = SliceDecart(tree, val);
  SlicersTree res_new = SliceDecart(res.max, val + 1);
  tree = MergeDecart(res.min, res_new.max );
  return tree;
}

ElemTree *KnumberStatistic(ElemTree *tree, int k)
{
  if(tree == NULL)  return NULL;

  ElemTree *left, *right;
  left = tree->left;
  right = tree->right;
  if((left != NULL) && (left->size >= (k + 1)))
  {
    return KnumberStatistic(left, k);
  }
  else if((left != NULL) && (left->size + 1) == (k + 1))
  {
    return tree;
  }
  else if((left == NULL) && ((tree->size - 1) == k))
  {
    return tree;
  }
  else if((left == NULL) && (k == 0)) return tree;
  else
  {
    int left_size;
    if(left == NULL)  left_size = 0;
    else    left_size = left->size;
    return KnumberStatistic(right, k - (left_size + 1));
  }
}

int Next(ElemTree *cur_el, int ogr)
{
  int res = POISON_VAL;
  int cnt = 0;
  while(cur_el != NULL)
  {
    cnt++;
    assert(cnt < LOOP_LIMIT);
    if(cur_el->val > ogr)
    {
      res    = cur_el->val;
      cur_el = cur_el->left;
    }
    else
    {
      cur_el = cur_el->right;
    }
  }
  return res;
}

int Prev(ElemTree *cur_el, int ogr)
{
  int res = POISON_VAL;
  int cnt = 0;
  while(cur_el != NULL)
  {
    cnt++;
    assert(cnt < LOOP_LIMIT);
    if(cur_el->val < ogr)
    {
      res = cur_el->val;
      cur_el = cur_el->right;
    }
    else
    {
      cur_el = cur_el->left;
    }
  }
  return res;
}

bool Exists(ElemTree *cur_el, int ogr)
{
  int res = POISON_VAL;
  int cnt = 0;
  while(cur_el != NULL)
  {
    cnt++;
    assert(cnt < LOOP_LIMIT);
    if(cur_el->val == ogr)
    {
      return true;
    }
    else if(cur_el->val < ogr)
    {
      cur_el = cur_el->right;
    }
    else if(cur_el->val > ogr)
    {
      cur_el = cur_el->left;
    }
  }
  return false;
}

int main()
{
  char str[20] = {};
  int a = 1;
  int val = 0, may = 0;
  ElemTree *arr = (ElemTree *)calloc(200000, sizeof(ElemTree));
  if(arr == NULL)  return -1;
  
  ElemTree *head = NULL, *ret;
  srand(time(NULL));
  int cnt = 0;

  while(a)
  {
    assert(cnt < LOOP_LIMIT);
    deep = 0;
    a = scanf("%s", str);
    if (a < 1) break;
    a = scanf("%d", &val);
    if (a < 1) break;

    if(strcmp(str, "insert") == 0)
    {
      cnt++;
      if(Exists(head, val) == false) {
        arr[may].val = val;
        arr[may].priorety = rand() % 200000;
        head = Insert(head, arr + may);
        may++;
      }
    }
    else if(strcmp(str, "exists") == 0)
    {
      bool res = Exists(head, val);
      printf("%s\n", res ? "true" : "false");
    }
    else if(strcmp(str, "delete") == 0)
    {
      if(Exists(head, val) == true) {
        head = Delete(head, val);
      }
    }
    else if(strcmp(str, "next") == 0)
    {
      int res = Next(head, val);
      if(res == POISON_VAL) printf("none\n");
      else              printf("%d\n", res);
    }
    else if(strcmp(str, "prev") == 0)
    {
      int res = Prev(head, val);
      if(res == POISON_VAL) printf("none\n");
      else               printf("%d\n", res);
    }
    else if(strcmp(str, "kth") == 0)
    {
      ret = KnumberStatistic(head, val);
      if(ret == NULL) printf("none\n");
      else            printf("%d\n", ret->val);
    }
    else
    {
      exit(-1);
    }
    
    memset(str, 0, 20);
  }
}
