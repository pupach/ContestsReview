#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PowerUniverse 2147483647
#define MAX_RAND 1000000000
#define MAX_SIZE_TABLE 10000000
#define MAX_SIZE_CONSTS 3
#define MAX(a, b) \
  (a > b ? a : b)
#define MAX_SIZE_COM 3
#define LOG(args...) \
       //   fprintf(stderr, args)

extern struct ElemTree;

typedef long long int ElemToUse;

typedef struct
{
    struct ElemTree *prev;
    long long int val;
    long long int deep;
    long long int delta;
    struct ElemTree* right;
    struct ElemTree* left;
}ElemTree;

typedef struct{
    ElemTree *head;

}AvlTree;

ElemTree *FindPlaceInTree(ElemTree *CurEl, long long int val)
{
  if(val == CurEl->val)
  {
    return CurEl;
  }
  else if(val >= CurEl->val)
  {
    if(CurEl->right == NULL)  return CurEl;
    return FindPlaceInTree(CurEl->right, val);
  }
  else{
    if(CurEl->left == NULL)  return CurEl;
    return FindPlaceInTree(CurEl->left, val);
  }
}

void SetHigh1(ElemTree *head)
{
  if(head == NULL)  return;
  ElemTree *left_tree = head->left;
  ElemTree *right_tree = head->right;
  if(right_tree == NULL)
  {
    if(left_tree != NULL) {
      head->deep = left_tree->deep + 1;
      head->delta = left_tree->deep;
    }
    else {
      head->deep = 1;
      head->delta = 0;
    }
  }
  else if(left_tree == NULL) {
    head->deep = right_tree->deep + 1;
    head->delta = -right_tree->deep;
  }
  else
  {
    head->deep = MAX(left_tree->deep, right_tree->deep) + 1;
    head->delta = left_tree->deep - right_tree->deep;
  }
}

void SetHigh(ElemTree *head)
{
  ElemTree *prev;

  SetHigh1(head);

  prev = head->prev;
  while(prev!= NULL)
  {
    SetHigh1(prev);
    prev = prev->prev;
  }
}

void RightRotate(ElemTree *head)
{
  ElemTree *another, *another_prev_head;
  ElemTree *to_rotate = head->right;
  ElemTree *prev_head = head->prev;
  ElemTree *to_change = NULL;
  if(to_rotate != NULL )    to_change = to_rotate->left;


  if(head->prev != NULL)
  {
    if (prev_head->left == head)
    {
      prev_head->left = to_rotate;
    }
    else
    {
      prev_head->right = to_rotate;
    }
  }

  to_rotate->prev = prev_head;
  to_rotate->left  = head;
  head->prev = to_rotate;


  if(to_change != NULL)   to_change->prev = head;
  head->right = to_change;

  SetHigh(head);
}

void LeftRotate(ElemTree *head)
{
  ElemTree *another, *another_prev_head;
  ElemTree *to_rotate = head->left;
  ElemTree *prev_head = head->prev;
  ElemTree *to_change = NULL;
  if(to_rotate != NULL )    to_change = to_rotate->right;

  if(head->prev != NULL)
  {
    if (prev_head->left == head)
    {
      prev_head->left = to_rotate;
    }
    else
    {
      prev_head->right = to_rotate;
    }
  }

  to_rotate->prev = prev_head;
  to_rotate->right  = head;
  head->prev = to_rotate;


  if(to_change != NULL)   to_change->prev = head;
  head->left = to_change;

  SetHigh(head);
}

void LeftBigRotate(ElemTree *head) {
  RightRotate(head->left);

  LeftRotate(head);
}

void RightBigRotate(ElemTree *head) {
  LeftRotate(head->right);

  RightRotate(head);
}

ElemTree *UpdateDeepTree(ElemTree *ElemToUpd)
{
  SetHigh(ElemToUpd);
  ElemTree *to_rotate, *may, *k;
  long long int deep_may, deep_k;

  if(ElemToUpd->delta > 1)
  {
    to_rotate = ElemToUpd->left;
    if(to_rotate!= NULL)
    {
      may = to_rotate->right;
      k   = to_rotate->left;

      if(may == NULL) deep_may = 0;
      else            deep_may = may->deep;

      if(k == NULL) deep_k = 0;
      else            deep_k = k->deep;

      LOG("BID_ROT_CHECK %d %d \n", deep_may, deep_k);

      if(deep_may > deep_k)
      {
        LeftBigRotate(ElemToUpd);
        if(ElemToUpd->prev != NULL)   return UpdateDeepTree(ElemToUpd->prev);
        else  return ElemToUpd;
      }
    }
    LeftRotate(ElemToUpd);
    SetHigh(ElemToUpd);
  }
  else if(ElemToUpd->delta < -1)
  {
    to_rotate = ElemToUpd->right;
    if(to_rotate!= NULL)
    {
      may = to_rotate->right;
      k   = to_rotate->left;

      if(may == NULL) deep_may = 0;
      else            deep_may = may->deep;

      if(k == NULL) deep_k = 0;
      else            deep_k = k->deep;

      LOG("BID_ROT_CHECK %d %d \n", deep_may, deep_k);

      if(deep_may < deep_k)
      {
        RightBigRotate(ElemToUpd);
        if(ElemToUpd->prev != NULL)   return UpdateDeepTree(ElemToUpd->prev);
        else  return ElemToUpd;
      }
    }
    RightRotate(ElemToUpd);
    SetHigh(ElemToUpd);
  }
  LOG("UpdateDeepTree cur_el = %d deep = %d delta_deep = %d]\n", ElemToUpd->val, ElemToUpd->deep, ElemToUpd->delta);

  if(ElemToUpd->prev != NULL)   return UpdateDeepTree(ElemToUpd->prev);
  else  return ElemToUpd;
}

void Insert(AvlTree *tree, ElemTree *ElemToIns)
{
  if(tree->head == NULL)
  {
    tree->head = ElemToIns;
    ElemToIns->deep = 1;
    return;
  }
  ElemTree *ptr = FindPlaceInTree(tree->head, ElemToIns->val);

  if(ElemToIns->val == ptr->val)
  {
    return;
  }
  else if(ElemToIns->val >= ptr->val)
  {
    ptr->right = ElemToIns;

  }
  else
  {
    ptr->left = ElemToIns;
  }
  ElemToIns->prev = ptr;
  ElemToIns->deep = 1;
  tree->head = UpdateDeepTree(ElemToIns->prev);
  ElemTree  *prev = ElemToIns->prev;
  LOG("Insert after %d HEAD = %d \n", prev->val, tree->head->val);
}


long long int FindMinEl(AvlTree *tree, long long int ogr)
{
  long long int res = -1;
  ElemTree *cur_el = tree->head;
  while(cur_el != NULL)
  {
    if(cur_el->val >= ogr)
    {
      res = cur_el->val;
      cur_el = cur_el->left;
    }
    else
    {
      cur_el = cur_el->right;
    }
  }
  return res;
}

int main()
{
  char str[20] = {};
  long long int AmountCom = 0, val = 0, may = 0;
  scanf("%lld", &AmountCom);
  ElemTree *arr = (ElemTree *)calloc(AmountCom, sizeof(ElemTree));
  AvlTree tree = {};

  for(long long int i = 0; i < AmountCom; i++) {
    scanf("%s", str);
    scanf("%lld", &val);
    switch (str[0]) {
      case '+':
        arr[i].val = (val + may) % 1000000000;
        may = 0;
        Insert(&tree, arr + i);
        UpdateDeepTree(tree.head);
        break;
      case '?':
        if(tree.head == NULL)
        {
          may = -1;
          printf("%lld\n", (long long int)-1);
        }
        else {
          may = FindMinEl(&tree, val);
          printf("%lld\n", may);
        }
        break;
    };
  }
}
