#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define PowerUniverse 2147483647
#define MAX_RAND 1000000000
#define MAX_SIZE_TABLE 10000000
#define MAX_SIZE_CONSTS 3
#define MAX_SIZE_COM 3
#define LOG(args...) \
    //      fprintf(stderr, args)

extern struct HashTable;

typedef int ElemToUse;

typedef struct
{
    struct HashTable *val;
    int *arr;
    int step;
    int size;
}ElemTable;

typedef struct{
    int size;
    int capacity;
    bool nul;
    int consts[2];
    int (*HashFunc)(struct HashTable *, ElemToUse);
    ElemTable *arr;

}HashTable;

int MainHashFunc(HashTable *table, ElemToUse ElenToHash)
{
  int hash = (((ElenToHash * table->consts[0] + table->consts[1])
              ) % table->consts[2] % table->capacity + table->capacity) % table->capacity;
  LOG("MainHashFunc hash = %d, ElenToHash = %d const0 = %d const1 = %d cop = %d\n", hash, ElenToHash, table->consts[0], table->consts[1], table->capacity);
  return hash;
}

void InitTable(HashTable *table, int capacity, int(*HashFunc)(struct HashTable *, ElemToUse), bool head_table)
{
  if(capacity == 0) return;
  table->capacity = capacity;
  table->size = 0;
  table->nul = false;
  table->arr = (ElemTable *)calloc(capacity, sizeof(ElemTable));
  if(head_table) {
    HashTable *arr = (HashTable *) calloc(capacity, sizeof(HashTable));
    for (int i = 0; i < capacity; i++) {
      table->arr[i].val = arr + i;
    }
  }
  table->HashFunc = HashFunc;
}

bool FindElemTable(HashTable *table, ElemToUse ElemToFind)
{
  if(table->capacity == 0)  return false;
  int hash = table->HashFunc(table, ElemToFind);
  if(table->arr[hash].size == 0)         return false;
  HashTable *t = table->arr[hash].val;

  if(t->capacity == 0)  return false;
  int hash_new = table->HashFunc(table->arr[hash].val, ElemToFind);
  if(t->arr[hash_new].size == 0)  return false;


  LOG("FindElemTable size = %d t->arr[hash_new].step = %d el = %d\n",t->arr[hash_new].size, t->arr[hash_new].arr[0] , ElemToFind);
  LOG("hash = %d new_hash = %d\n", hash, hash_new);
  if(t->arr[hash_new].arr[0] != ElemToFind)
  {return false;}
  return true;
}

int FindCollision(HashTable *table, int* val, int len_val);

void GenFirstHashFunc(HashTable *table, int *val, int AmountCom, int ogranich)
{
  int cnt1 = 0;
  if(table->capacity == 0)  return;
  if(ogranich == 0)  return;
  if(AmountCom == 0)  return;
  int cnt = 0;
  do{
    cnt1++;
    if((cnt1 % 100) == 0)
    {

      if(cnt1 == 200)
      {
        //exit(-1);
      }
    }
    table->consts[0] = rand() % table->capacity;
    table->consts[1] = rand() % table->capacity;
    table->consts[2] = (rand() + table->capacity) % (3 * table->capacity + 1) + 1;
    LOG("const 1 = %d 2 = %d\n", table->consts[0], table->consts[1]);
    cnt = FindCollision(table, val, AmountCom);

    LOG("\n GenFirstHashFunc cnt = %d, ogranich = %d \n", cnt, ogranich);
  }while(cnt > ogranich);

  for(int i = 0; i < AmountCom; i++)
  {
    int hash = table->HashFunc(table, val[i]);
    table->arr[hash].size++;
  }

  for(int i = 0; i < table->capacity; i++) {
    if (table->arr[i].size != 0) {
      table->arr[i].arr = (int *) calloc(sizeof(int), table->arr[i].size);
      InitTable(table->arr[i].val, table->arr[i].size * 4, &MainHashFunc, true);

    }
  }
  for(int i = 0; i < AmountCom; i++)
  {
    int hash = table->HashFunc(table, val[i]);
    table->arr[hash].arr[table->arr[hash].step] = val[i];
    table->arr[hash].step++;
  }
}

int FindCollision(HashTable *table, int* val, int len_val)
{
  int *arr = (int *)calloc(table->capacity, sizeof(int));
  LOG("begin FindCollision table->capacity = %d\n", table->capacity);
  for(int i = 0; i < len_val; i++)
  {
    int hash = table->HashFunc(table, val[i]);
    LOG("hash = %d val = %d |", hash, val[i]);
    arr[hash]++;
  }
  int cnt = 0;
  for(int i = 0; i < table->capacity; i++)
  {
    cnt += (arr[i] * arr[i]);
  }
  free(arr);
  return cnt;
}

int main()
{
  char str[MAX_SIZE_COM] = {};
  int AmountCom = 0;
  scanf("%d", &AmountCom);
  if(AmountCom > 4)
  {
//      exit(-1);
  }
  int *val = (int *)calloc(sizeof(int), AmountCom);
  HashTable table = {};
  InitTable(&table, AmountCom, &MainHashFunc, true);
  srand(time(NULL));
  for(int i = 0; i < AmountCom; i++)
  {
    scanf("%d", val + i);
    if(val[i] == 0)
    {
      table.nul = true;
    }
  }
  GenFirstHashFunc(&table, val, AmountCom, 4 * table.capacity);
  for(int i = 0; i < table.capacity; i++)
  {
    GenFirstHashFunc(table.arr[i].val, table.arr[i].arr, table.arr[i].step, table.arr[i].size);
  }

  do{
    scanf("%s", str);
    if(str[0] == '.')  break;
    int numb = atoi(str);
    bool res = FindElemTable(&table, numb);
    if(str[0] == '0') res = table.nul;
    if(res) printf("YES\n");
    else    printf("NO\n");

  }while(str[0] != '.');
}
