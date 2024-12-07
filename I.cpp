
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
    compr_arr[i][1] = (int)new_val;
    compr_arr[i][0] = i;
  }
  return compr_arr;
}

struct Node{
 public:
  int l_value  = 0;
  int r_value  = 0;
  int l_parent = 0;
  int r_parent = 0;

  Node(int n) {}
};
class TreeSeg{
 public:
  std::vector<Node> tree;
  std::vector<int> r_parent;
  std::vector<int> l_parent;

  TreeSeg(int n, int c) : tree(c, Node(0)), r_parent(n, -1), l_parent(n, -1) {}
  void left_val_update(int val, int ind_to_update, int who);
  void right_val_update(int val, int ind_to_update, int who);
  int left_val_get_sum(int l, int r, int v, int tl, int tr);
  int right_val_get_sum(int l, int r, int v, int tl, int tr);
  int left_val_find(int elem, int cur_v);
  int right_val_find(int elem, int cur_v);
};

void TreeSeg::left_val_update(int val, int ind_to_update, int who) {
  if(ind_to_update < (tree.size() / 2))
  {
    if(tree[ind_to_update * 2 + 2].l_value > tree[ind_to_update * 2 + 1].l_value)
    {
      tree[ind_to_update].l_parent = tree[ind_to_update * 2 + 2].l_parent;
      tree[ind_to_update].l_value  = tree[ind_to_update * 2 + 2].l_value;
    }
    else{
      tree[ind_to_update].l_parent = tree[ind_to_update * 2 + 1].l_parent;
      tree[ind_to_update].l_value  = tree[ind_to_update * 2 + 1].l_value;
    }

  }
  else
  {
    tree[ind_to_update].l_parent = who;
    tree[ind_to_update].l_value = val;
  }
  if(ind_to_update != 0) {left_val_update(val, (ind_to_update - 1) / 2, who);}
}

void TreeSeg::right_val_update(int val, int ind_to_update, int who) {
  if(ind_to_update < (tree.size() / 2))
  {
    if(tree[ind_to_update * 2 + 2].r_value > tree[ind_to_update * 2 + 1].r_value)
    {
      tree[ind_to_update].r_parent = tree[ind_to_update * 2 + 2].r_parent;
      tree[ind_to_update].r_value  = tree[ind_to_update * 2 + 2].r_value;
    }
    else{
      tree[ind_to_update].r_parent = tree[ind_to_update * 2 + 1].r_parent;
      tree[ind_to_update].r_value  = tree[ind_to_update * 2 + 1].r_value;
    }

  }
  else
  {
    tree[ind_to_update].r_parent = who;
    tree[ind_to_update].r_value = val;
  }
  if(ind_to_update != 0) {right_val_update(val, (ind_to_update - 1) / 2, who);}
}


int TreeSeg::left_val_find(int elem, int cur_v)
{
  if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
  if(tree[cur_v * 2 + 2].l_value == elem) return left_val_find(elem, cur_v * 2 + 2);
  else                            return left_val_find(elem, cur_v * 2 + 1);
}

int TreeSeg::right_val_find(int elem, int cur_v)
{
  if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
  if(tree[cur_v * 2 + 2].r_value == elem) return right_val_find(elem, cur_v * 2 + 2);
  else                            return right_val_find(elem, cur_v * 2 + 1);
}

int TreeSeg::left_val_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
  if(right == -1) return -1;
  if(left > right) return -1;
  if ((left <= cur_l) && (cur_r <= right)) {
    return left_val_find(tree[cur_v].l_value, cur_v);
  }
  if ((cur_r < left) || (right < cur_l)) {
    return -1;
  }

  int tm = (cur_l + cur_r) / 2;
  int second, first;

  int f = left_val_get_sum(left, right, cur_l, tm, cur_v * 2 + 1);

  if(f == -1) first = 0;
  else first = left_val_find(tree[f].l_value, f);

  int s = left_val_get_sum(left, right,tm + 1, cur_r, cur_v * 2 + 2);
  if(s == -1) second = 0;
  else second = left_val_find(tree[s].l_value, s);

  if(f == -1) return second;
  if(s == -1) return first;

  if(tree[first].l_value >= tree[second].l_value)   return first;
  else                    return second;
}

int TreeSeg::right_val_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
  if(right == -1) return -1;
  if(left > right) return -1;
  if ((left <= cur_l) && (cur_r <= right)) {
    return right_val_find(tree[cur_v].r_value, cur_v);
  }

  if ((cur_r < left) || (right < cur_l)) {
    return -1;
  }

  int tm = (cur_l + cur_r) / 2;
  int second, first;

  int f = right_val_get_sum(left, right, cur_l, tm, cur_v * 2 + 1);

  if(f == -1) first = 0;
  else first = right_val_find(tree[f].r_value, f);

  int s = right_val_get_sum(left, right,tm + 1, cur_r, cur_v * 2 + 2);
  if(s == -1) second = 0;
  else second = right_val_find(tree[s].r_value, s);
  if(f == -1) return second;
  if(s == -1) return first;

  if(tree[first].r_value >= tree[second].r_value)   return first;
  else                    return second;
}

void FindRightWay(std::vector<int>& way, TreeSeg& tree_seg, int mod_2)
{
  int ind_max = -1;
  if(mod_2 == 0) ind_max = tree_seg.tree[0].r_parent;
  else ind_max = tree_seg.tree[0].l_parent;

  int ptr_way = way.size() - 1;
  while (ind_max != -1) {
    way[ptr_way] = ind_max;
    ptr_way--;
    if((mod_2 % 2) == 0) {
      ind_max = tree_seg.l_parent[ind_max];
    }
    else{
      ind_max = tree_seg.r_parent[ind_max];
    }
    mod_2++;
  }
}
int main(){
  int n = 0;
  std::cin >> n;
  std::vector<std::array<long long int, NUMB_ELEM>> vector_helper(n);
  std::vector<std::array<long long int, NUMB_ELEM>> set_sort_vect;
  std::vector<std::array<long long int, NUMB_ELEM>> init_vector;

  for(int i = 0; i < n; i++)
  {
    std::cin >> vector_helper[i][1];
    vector_helper[i][0] = i;
  }
  init_vector = vector_helper;
  std::sort(vector_helper.begin(), vector_helper.end(), [](std::array<long long int, NUMB_ELEM> f, std::array<long long int, NUMB_ELEM>s)-> int
  {
    return f[1] > s[1];
  });

  set_sort_vect.push_back(vector_helper[0]);
  for(int i = 1; i < vector_helper.size(); i++)
  {
    if(vector_helper[i][1] != vector_helper[i - 1][1])
    {
      set_sort_vect.push_back(vector_helper[i]);
    }
  }
  int c = 1;
  while(c < 2 * set_sort_vect.size() - 1)
  {
    c *= 2;
  }
  TreeSeg tree_seg(n, c - 1);
  if(c == 1) {
    std::cout << 1 << '\n';
    std::cout << init_vector[0][1];
  }
  else{
    std::vector<std::array<int, 2>> compr_vect = CompressionCoor(init_vector, set_sort_vect);

    for (int i = 0; i < n; i++)
    {
      int ind_sum = tree_seg.right_val_get_sum(0, compr_vect[i][1] - 1, 0, (c / 2) - 1, 0);
      if ((ind_sum != -1) && (tree_seg.tree[ind_sum].r_value != 0)) tree_seg.r_parent[i] = tree_seg.tree[ind_sum].r_parent;

      int sum_to_up = 0;
      if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].r_value;
      tree_seg.left_val_update(sum_to_up + 1, (int) (c / 2) - 1 + compr_vect[i][1], i);

      ind_sum = tree_seg.left_val_get_sum(compr_vect[i][1] + 1, (c / 2) - 1, 0, (c / 2) - 1, 0);
      if ((ind_sum != -1) && (tree_seg.tree[ind_sum].l_value != 0)) tree_seg.l_parent[i] = tree_seg.tree[ind_sum].l_parent;

      sum_to_up = 0;
      if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].l_value;
      tree_seg.right_val_update(sum_to_up + 1, (int) (c / 2) - 1 + compr_vect[i][1], i);
    }

    std::cout << std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value)  << '\n';

    std::vector<int> way(std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value), 0);

    if(tree_seg.tree[0].l_value > tree_seg.tree[0].r_value) {
      FindRightWay(way, tree_seg, 1);
    }
    else{
      FindRightWay(way, tree_seg, 0);
    }

    std::cout << init_vector[way[0]][1] << ' ';
    for (int i = 1; i < std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value); i++) {
      std::cout << init_vector[way[i]][1] << ' ';
      assert(way[i] != 0);
    }
  }
}
