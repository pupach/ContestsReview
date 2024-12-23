
#include<algorithm>
#include<bits/stdc++.h>
#include<iostream>
#include<stdlib.h>
#include<vector>

const int NUMB_ELEM = 2;


std::size_t bin_search(std::vector<std::array<long long int, NUMB_ELEM>>& arr, long long int elem, std::size_t begin, std::size_t end)
{
  if(begin == end) return begin;
  if(elem < arr[(begin + end) / 2][1])
  {
    return bin_search(arr, elem, (begin + end) / 2 + 1, end);
  }
  else if(elem > arr[(begin + end) / 2][1])
  {
    return bin_search(arr, elem, begin, (begin + end) / 2 - 1);
  }
  else return bin_search(arr, elem, begin, (begin + end) / 2);
}

std::vector<std::array<int, 2>> CompressionCoor(std::vector<std::array<long long int, 2>>& orig_arr, std::vector<std::array<long long int, 2>>& sort_arr)
{
  std::vector<std::array<int, 2>> compr_arr(orig_arr.size());

  for(int i = 0; i < orig_arr.size(); i++)
  {
    size_t new_val = bin_search(sort_arr, orig_arr[i][1], 0, sort_arr.size());
    compr_arr[i][1] = static_cast<int>(new_val);
    compr_arr[i][0] = i;
  }
  return compr_arr;
}

struct Node{
 public:
  int left_value  = 0;
  int right_value  = 0;
  int left_parent = 0;
  int right_parent = 0;

  Node(int n) {}
};
class TreeSeg{
 public:
  std::size_t amount_gangsters;
  std::vector<Node> tree;
  std::vector<int> right_parent;
  std::vector<int> left_parent;


  TreeSeg(std::size_t n, int c) : amount_gangsters(n), tree(c, Node(0)), right_parent(n, -1), left_parent(n, -1) {}
  void left_val_update(int val, int ind_to_update, int who);
  void right_val_update(int val, int ind_to_update, int who);
  int left_val_get_sum(int l, int r, int v, int tl, int tr);
  int right_val_get_sum(int l, int r, int v, int tl, int tr);
  int left_val_find(int elem, int cur_v);
  int right_val_find(int elem, int cur_v);
  void FindRightWay(std::vector<int>& way, int mod_2);
  void FillTree();
};

void TreeSeg::FillTree() {
  std::vector<std::array<int, 2>> compr_vect = CompressionCoor(init_vector, arr_sort_vect);

  for (std::size_t i = 0; i < amount_gangsters; i++)
  {
    int ind_sum = tree_seg.right_val_get_sum(0, compr_vect[i][1] - 1, 0, (c / 2) - 1, 0);
    if ((ind_sum != -1) && (tree_seg.tree[ind_sum].right_value != 0)) tree_seg.right_parent[i] = tree_seg.tree[ind_sum].right_parent;

    int sum_to_up = 0;
    if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].right_value;
    tree_seg.left_val_update(sum_to_up + 1, c / 2 - 1 + compr_vect[i][1], i);

    ind_sum = tree_seg.left_val_get_sum(compr_vect[i][1] + 1, (c / 2) - 1, 0, (c / 2) - 1, 0);
    if ((ind_sum != -1) && (tree_seg.tree[ind_sum].left_value != 0)) tree_seg.left_parent[i] = tree_seg.tree[ind_sum].left_parent;

    sum_to_up = 0;
    if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].left_value;
    tree_seg.right_val_update(sum_to_up + 1, c / 2 - 1 + compr_vect[i][1], i);
  }
};

void TreeSeg::left_val_update(int val, int ind_to_update, int who) {
  if(ind_to_update < (tree.size() / 2))
  {
    if(tree[ind_to_update * 2 + 2].left_value > tree[ind_to_update * 2 + 1].left_value)
    {
      tree[ind_to_update].left_parent = tree[ind_to_update * 2 + 2].left_parent;
      tree[ind_to_update].left_value  = tree[ind_to_update * 2 + 2].left_value;
    }
    else{
      tree[ind_to_update].left_parent = tree[ind_to_update * 2 + 1].left_parent;
      tree[ind_to_update].left_value  = tree[ind_to_update * 2 + 1].left_value;
    }

  }
  else
  {
    tree[ind_to_update].left_parent = who;
    tree[ind_to_update].left_value = val;
  }
  if(ind_to_update != 0) {left_val_update(val, (ind_to_update - 1) / 2, who);}
}

void TreeSeg::right_val_update(int val, int ind_to_update, int who) {
  if(ind_to_update < (tree.size() / 2))
  {
    if(tree[ind_to_update * 2 + 2].right_value > tree[ind_to_update * 2 + 1].right_value)
    {
      tree[ind_to_update].right_parent = tree[ind_to_update * 2 + 2].right_parent;
      tree[ind_to_update].right_value  = tree[ind_to_update * 2 + 2].right_value;
    }
    else{
      tree[ind_to_update].right_parent = tree[ind_to_update * 2 + 1].right_parent;
      tree[ind_to_update].right_value  = tree[ind_to_update * 2 + 1].right_value;
    }

  }
  else
  {
    tree[ind_to_update].right_parent = who;
    tree[ind_to_update].right_value = val;
  }
  if(ind_to_update != 0) {right_val_update(val, (ind_to_update - 1) / 2, who);}
}


int TreeSeg::left_val_find(int elem, int cur_v)
{
  if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
  if(tree[cur_v * 2 + 2].left_value == elem) return left_val_find(elem, cur_v * 2 + 2);
  else                            return left_val_find(elem, cur_v * 2 + 1);
}

int TreeSeg::right_val_find(int elem, int cur_v)
{
  if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
  if(tree[cur_v * 2 + 2].right_value == elem) return right_val_find(elem, cur_v * 2 + 2);
  else                            return right_val_find(elem, cur_v * 2 + 1);
}

int TreeSeg::left_val_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
  if(right == -1) return -1;
  if(left > right) return -1;
  if ((left <= cur_l) && (cur_r <= right)) {
    return left_val_find(tree[cur_v].left_value, cur_v);
  }
  if ((cur_r < left) || (right < cur_l)) {
    return -1;
  }

  int middle = (cur_l + cur_r) / 2;
  int second, first;

  int left_sum = left_val_get_sum(left, right, cur_l, middle, cur_v * 2 + 1);

  if(left_sum == -1) first = 0;
  else first = left_val_find(tree[left_sum].left_value, left_sum);

  int right_sum = left_val_get_sum(left, right,middle + 1, cur_r, cur_v * 2 + 2);
  if(right_sum == -1) second = 0;
  else second = left_val_find(tree[right_sum].left_value, right_sum);

  if(left_sum == -1) return second;
  if(right_sum == -1) return first;

  if(tree[first].left_value >= tree[second].left_value)   return first;
  else                    return second;
}

int TreeSeg::right_val_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
  if(right == -1) return -1;
  if(left > right) return -1;
  if ((left <= cur_l) && (cur_r <= right)) {
    return right_val_find(tree[cur_v].right_value, cur_v);
  }

  if ((cur_r < left) || (right < cur_l)) {
    return -1;
  }

  int middle = (cur_l + cur_r) / 2;
  int second, first;

  int left_sum = right_val_get_sum(left, right, cur_l, middle, cur_v * 2 + 1);

  if(left_sum == -1) first = 0;
  else first = right_val_find(tree[left_sum].right_value, left_sum);

  int right_sum = right_val_get_sum(left, right,middle + 1, cur_r, cur_v * 2 + 2);
  if(right_sum == -1) second = 0;
  else second = right_val_find(tree[right_sum].right_value, right_sum);
  if(left_sum == -1) return second;
  if(right_sum == -1) return first;

  if(tree[first].right_value >= tree[second].right_value)   return first;
  return second;
}

void TreeSeg::FindRightWay(std::vector<int>& way, int mod_2)
{
  int ind_max = -1;
  if(mod_2 == 0) ind_max = tree[0].right_parent;
  else ind_max = tree[0].left_parent;

  int ptr_way = way.size() - 1;
  while (ind_max != -1) {
    way[ptr_way] = ind_max;
    ptr_way--;
    if((mod_2 % 2) == 0) {
      ind_max = left_parent[ind_max];
    }
    else{
      ind_max = right_parent[ind_max];
    }
    mod_2++;
  }
}

int LogarifmTwo(int number)
{
  int c = 1;
  while(c < number)
  {
    c *= 2;
  }
  return c;
}

void OutputInCout(const vect_of_arr& init_vector, const std::vector<int>& way, int range) {
  std::cout << init_vector[way[0]][1] << ' ';
  for (int i = 1; i < range; i++) {
    std::cout << init_vector[way[i]][1] << ' ';
    assert(way[i] != 0);
  }
}

int main(){
  std::size_t n = 0;
  std::cin >> n;
  using vect_of_arr = std::vector<std::array<long long int, NUMB_ELEM>>;
  vect_of_arr vector_helper(n);
  vect_of_arr arr_sort_vect;
  vect_of_arr init_vector;

  for(std::size_t i = 0; i < n; i++)
  {
    std::cin >> vector_helper[i][1];
    vector_helper[i][0] = i;
  }
  init_vector = vector_helper;
  std::sort(vector_helper.begin(), vector_helper.end(), [](std::array<long long int, NUMB_ELEM> left_sum, std::array<long long int, NUMB_ELEM>right_sum)-> int
  {
    return left_sum[1] > right_sum[1];
  });

  arr_sort_vect.push_back(vector_helper[0]);
  for(std::size_t i = 1; i < vector_helper.size(); i++)
  {
    if(vector_helper[i][1] != vector_helper[i - 1][1])
    {
      arr_sort_vect.push_back(vector_helper[i]);
    }
  }
  int c = LogarifmTwo(2 * arr_sort_vect.size() - 1);

  TreeSeg tree_seg(n, c - 1);

  if(c == 1) {
    std::cout << 1 << '\n';
    std::cout << init_vector[0][1];
  }
  else{
    tree_seg.FillTree();

    std::cout << std::max(tree_seg.tree[0].left_value, tree_seg.tree[0].right_value)  << '\n';

    std::vector<int> way(std::max(tree_seg.tree[0].left_value, tree_seg.tree[0].right_value), 0);
    if(tree_seg.tree[0].left_value > tree_seg.tree[0].right_value) {
      tree_seg.FindRightWay(way, 1);
    }
    else{
      tree_seg.FindRightWay(way, 0);
    }
    OutputInCout(init_vector, way, std::max(tree_seg.tree[0].left_value, tree_seg.tree[0].right_value));
  }
}
