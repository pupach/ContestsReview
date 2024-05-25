#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define INIT_X 100000
#define INIT_Y 600
#define LOG(args...) \
       //   fprintf(stderr, args)

extern struct ElemTree;

typedef long long int ElemToUse;

typedef struct
{
  struct  ElemTree  *prev;
  char              *val_f;
  char              *val_s;
  struct  ElemTree  *right;
  struct  ElemTree  *left;
} ElemTree;

typedef struct{
  ElemTree *min;
  ElemTree *max;
} SlicersTree;

ElemTree *GetParent(ElemTree *elem)
{
  return (elem == NULL) ? NULL : elem->prev;
}

void SetParent(ElemTree *elem, ElemTree *parent)
{
  if(elem != NULL) elem->prev = parent;
}

void LeftRotate(ElemTree *head)
{
  ElemTree *another, *another_prev_head;
  ElemTree *to_rotate = head->right;
  ElemTree *prev_head = head->prev;
  ElemTree *to_change = NULL;
  
  if(to_rotate != NULL )    to_change = to_rotate->left;

  if(head->prev != NULL)
  {
    (prev_head->left == head) ? (prev_head->left = to_rotate) : (prev_head->right = to_rotate);
  }

  to_rotate->prev = prev_head;
  to_rotate->left  = head;
  head->prev = to_rotate;

  if(to_change != NULL)   to_change->prev = head;
  head->right = to_change;
}

void RightRotate(ElemTree *head)
{
  ElemTree *another, *another_prev_head;
  ElemTree *to_rotate = head->left;
  ElemTree *prev_head = head->prev;
  ElemTree *to_change = NULL;
  
  if(to_rotate != NULL )    to_change = to_rotate->right;

  if(head->prev != NULL)
  {
    (prev_head->left == head) ? (prev_head->left = to_rotate) : (prev_head->right = to_rotate);
  }

  to_rotate->prev = prev_head;
  to_rotate->right  = head;
  head->prev = to_rotate;

  if(to_change != NULL)   to_change->prev = head;
  head->left = to_change;
}

ElemTree *BalanceTree(ElemTree *root_to_zig)
{
  ElemTree *father      = GetParent(root_to_zig);
  ElemTree *grandfather = GetParent(father);
  
  if(father == NULL)  return root_to_zig;
  else if(grandfather == NULL)
  {
    if(father->left == root_to_zig)
    {
      RightRotate(father);
    }
    else if(father->right == root_to_zig)
    {
      LeftRotate(father);
    }
    else
    {
      LOG("ERRRRRORRRRR\n");
    }
  }
  else if((grandfather->left == father) && (father->left == root_to_zig))
  {
    RightRotate(grandfather);
    RightRotate(father);
  }
  else if((grandfather->right == father) && (father->right == root_to_zig))
  {
    LeftRotate(grandfather);
    LeftRotate(father);
  }
  else if((grandfather->left == father) && (father->right == root_to_zig))
  {
    LeftRotate(father);
    RightRotate(grandfather);
  }
  else if((grandfather->right == father) && (father->left == root_to_zig))
  {
    RightRotate(father);
    LeftRotate(grandfather);
  }
  else
  {
    LOG("ERRRRRORRRRR\n");
  }
  return root_to_zig;
}

ElemTree *Splay(ElemTree *elem)
{
  while(elem->prev != NULL)
  {
    BalanceTree(elem);
  }
  return elem;
}


int CmpTreeElem(ElemTree *f, char *val)
{
  return strcmp(f->val_f, val);
}


ElemTree *FindMinEl(ElemTree *tree, char *elem)
{
  if(tree == NULL)  return NULL;

  if(CmpTreeElem(tree, elem) == 0)
  {
    Splay(tree);
    return tree;
  }
  else if((CmpTreeElem(tree, elem) > 0) && (tree->left != NULL))
  {
    return FindMinEl(tree->left, elem);
  }
  else if((CmpTreeElem(tree, elem) < 0) && (tree->right != NULL))
  {
    return FindMinEl(tree->right, elem);
  }
  else
  {
    return Splay(tree);
  }
}

SlicersTree Split(ElemTree *tree, char *elem)
{
  SlicersTree ret = {};
  if(tree == NULL)  return ret;

  tree = FindMinEl(tree, elem);
  if((CmpTreeElem(tree, elem) > 0))
  {
    SetParent(tree->left, NULL);

    ret.min = tree->left;
    ret.max = tree;
    tree->left = NULL;
    return ret;
  }
  else if((CmpTreeElem(tree, elem) < 0))
  {
    SetParent(tree->right, NULL);

    ret.max = tree->right;
    ret.min = tree;
    tree->right = NULL;
    return ret;
  }
}

ElemTree *Insert(ElemTree *tree, ElemTree *ElemToIns)
{
  SlicersTree ret = Split(tree, ElemToIns->val_f);

  SetParent(ret.min, ElemToIns);
  SetParent(ret.max, ElemToIns);
  ElemToIns->left = ret.min;
  ElemToIns->right = ret.max;

  return ElemToIns;
}

ElemTree *Merge(ElemTree *left, ElemTree *right)
{
  if(left == NULL)  return  right;
  if(right == NULL) return  left;

  right = FindMinEl(right, left->val_f);
  right->left = left;
  SetParent(left, right);
  return right;
}

ElemTree *Remove(ElemTree *head, char *elem)
{
  head = FindMinEl(head, elem);
  if(CmpTreeElem(head, elem) != 0)  return head;
  else
  {
    SetParent(head->left, NULL);
    SetParent(head->right, NULL);

    return Merge(head->left, head->right);
  }
}

int main()
{
  char str_f[INIT_X][INIT_Y] = {};
  char str_s[INIT_X][INIT_Y] = {};
  char quest[INIT_X][INIT_Y] = {};
  int AmountCom = 0, val = 0, may = 0;
  scanf("%d", &AmountCom);


  ElemTree *arr_f = (ElemTree *)calloc(AmountCom, sizeof(ElemTree));
  if(arr_f == NULL) return -1;
       
  ElemTree *arr_s = (ElemTree *)calloc(AmountCom, sizeof(ElemTree));
  if(arr_f == NULL) return -1;
  ElemTree *head_f = NULL;

  for(int i = 0; i < AmountCom; i++) {
    scanf("%s %s", str_f[i], str_s[i]);

    arr_f[i].val_f = str_f[i];
    arr_f[i].val_s = str_s[i];
    arr_s[i].val_f = str_s[i];
    arr_s[i].val_s = str_f[i];

    head_f = Insert(head_f, arr_s + i);
    head_f = Insert(head_f, arr_f + i);
    assert(head_f == arr_f + i);
  }

  scanf("%d", &AmountCom);

  for(int i = 0; i < AmountCom; i++) {
    scanf("%s", quest[i]);

    head_f =  FindMinEl(head_f, quest[i]);

    if(CmpTreeElem(head_f, quest[i]) == 0) printf("%s\n", head_f->val_s);
  }
}
