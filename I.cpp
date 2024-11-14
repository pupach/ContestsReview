#include<iostream>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include <cstdlib>
#include<bits/stdc++.h>
using namespace std;

size_t bin_search(vector<std::array<long long int, 2>>& arr, long long int elem, size_t begin, size_t end)
{
    //fprintf(stderr,"bin_search brgin elem = %lld, begin = %zu, end = %zu\n", elem, begin, end);
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
/*
int find_correct_elem(vector<std::array<int, 2>>& arr, int begin, int to_find)
{
    for(int i = begin; i < arr.size(); i++)
    {
        if(arr[i][0] == to_find)    return i;
    }
    return 0;
}
*/
vector<std::array<int, 2>> CompressionCoor(vector<std::array<long long int, 2>>& orig_arr, vector<std::array<long long int, 2>>& sort_arr)
{
    vector<std::array<int, 2>> compr_arr(orig_arr.size());
    //std::cerr <<"CompressionCoor compr_arr " << '\n';

    for(int i = 0; i < orig_arr.size(); i++)
    {
        //int new_val = find_correct_elem(sort_arr, bin_search(sort_arr, orig_arr[i][1], 0, orig_arr.size()), orig_arr[i][0]);
        //std::cerr <<"bin_search begin i = " << i << '\n';
        size_t new_val = bin_search(sort_arr, orig_arr[i][1], 0, sort_arr.size());
        //std::cerr <<"bin_search begin new_val =  "<< new_val << '\n';
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
    vector<Node> tree;
    vector<int> r_parent;
    vector<int> l_parent;

    TreeSeg(int n, int c) : tree(c, Node(0)), r_parent(n, -1), l_parent(n, -1) {}
    void l_update(int val, int ind_to_update, int who);
    void r_update(int val, int ind_to_update, int who);
    int l_get_sum(int l, int r, int v, int tl, int tr);
    int r_get_sum(int l, int r, int v, int tl, int tr);
    int l_find(int elem, int cur_v);
    int r_find(int elem, int cur_v);
};

void TreeSeg::l_update(int val, int ind_to_update, int who) {
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
    if(ind_to_update != 0) {l_update(val, (ind_to_update - 1) / 2, who);}
}

void TreeSeg::r_update(int val, int ind_to_update, int who) {
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
    if(ind_to_update != 0) {r_update(val, (ind_to_update - 1) / 2, who);}
}


int TreeSeg::l_find(int elem, int cur_v)
{
    if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
    if(tree[cur_v * 2 + 2].l_value == elem) return l_find(elem, cur_v * 2 + 2);
    else                            return l_find(elem, cur_v * 2 + 1);
}

int TreeSeg::r_find(int elem, int cur_v)
{
    if((tree.size() / 2 - 1) <= cur_v)    return cur_v;
    if(tree[cur_v * 2 + 2].r_value == elem) return r_find(elem, cur_v * 2 + 2);
    else                            return r_find(elem, cur_v * 2 + 1);
}

int TreeSeg::l_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
    //std::cerr << "get_sum begin left = " << left << " right = " << right << " cur_l = "<< cur_l << " cur_r = "<< cur_r << " cur_v = "<< cur_v << '\n';
    //вариант 1
    if(right == -1) return -1;
    if(left > right) return -1;
    if ((left <= cur_l) && (cur_r <= right)) {
        //std::cerr << "get_sum end return = " << tree[cur_v].l_value + tree.size() / 2<< '\n';
        return l_find(tree[cur_v].l_value, cur_v);
        //return tree[cur_v][0] + tree.size() / 2;
    }

    //вариант 2
    if ((cur_r < left) || (right < cur_l)) {
        return -1;
    }

    int tm = (cur_l + cur_r) / 2;
    int ss, ff;

    int f = l_get_sum(left, right, cur_l, tm, cur_v * 2 + 1);

    if(f == -1) ff = 0;
    else ff = l_find(tree[f].l_value, f);
    //int ff = tree[f][0] + tree.size() / 2 ;

    int s = l_get_sum(left, right,tm + 1, cur_r, cur_v * 2 + 2);
    if(s == -1) ss = 0;
    else ss = l_find(tree[s].l_value, s);
    //int ss = tree[s][0] + tree.size() / 2;
    //std::cerr << "ff =" << ff << "ss = " << ss << '\n';

    if(f == -1) return ss;
    if(s == -1) return ff;

    if(tree[ff].l_value >= tree[ss].l_value)   return ff;
    else                    return ss;
}

int TreeSeg::r_get_sum(int left, int right, int cur_l, int cur_r, int cur_v) {
    //std::cerr << "get_sum begin left = " << left << " right = " << right << " cur_l = "<< cur_l << " cur_r = "<< cur_r << " cur_v = "<< cur_v << '\n';
    //вариант 1
    if(right == -1) return -1;
    if(left > right) return -1;
    if ((left <= cur_l) && (cur_r <= right)) {
        //std::cerr << "get_sum end return = " << tree[cur_v].r_value + tree.size() / 2<< '\n';
        return r_find(tree[cur_v].r_value, cur_v);
        //return tree[cur_v][0] + tree.size() / 2;
    }

    //вариант 2
    if ((cur_r < left) || (right < cur_l)) {
        return -1;
    }

    int tm = (cur_l + cur_r) / 2;
    int ss, ff;

    int f = r_get_sum(left, right, cur_l, tm, cur_v * 2 + 1);

    if(f == -1) ff = 0;
    else ff = r_find(tree[f].r_value, f);
    //int ff = tree[f][0] + tree.size() / 2 ;

    int s = r_get_sum(left, right,tm + 1, cur_r, cur_v * 2 + 2);
    if(s == -1) ss = 0;
    else ss = r_find(tree[s].r_value, s);
    //int ss = tree[s][0] + tree.size() / 2;
    //std::cerr << "ff =" << ff << "ss = " << ss << '\n';

    if(f == -1) return ss;
    if(s == -1) return ff;

    if(tree[ff].r_value >= tree[ss].r_value)   return ff;
    else                    return ss;
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
    std::vector<std::array<long long int, 2>> meow(n);
    std::vector<std::array<long long int, 2>> set_sort_vect;
    std::vector<std::array<long long int, 2>> vect2;

    for(int i = 0; i < n; i++)
    {
        std::cin >> meow[i][1];
        meow[i][0] = i;
    }
    vect2 = meow;
    // std::cerr << 1 << '\n';
    std::sort(meow.begin(), meow.end(), [](std::array<long long int, 2> f, std::array<long long int, 2>s)-> int
    {
        return f[1] > s[1];
    });
    //std::cerr << 1 << '\n';

    set_sort_vect.push_back(meow[0]);
    for(int i = 1; i < meow.size(); i++)
    {
        if(meow[i][1] != meow[i - 1][1])
        {
            set_sort_vect.push_back(meow[i]);
        }
    }
    //std::cerr << 1 << '\n';
    int c = 1;
    while(c < 2 * set_sort_vect.size() - 1)
    {
        c *= 2;
    }
    //std::cerr << 1 << '\n';
    TreeSeg tree_seg(n, c - 1);
    if(c == 1) {
        std::cout << 1 << '\n';
        std::cout << vect2[0][1];
    }
    else{
        //std::cerr <<"CompressionCoor begin " << '\n';
        std::vector<std::array<int, 2>> compr_vect = CompressionCoor(vect2, set_sort_vect);
        //std::cerr <<"CompressionCoor end " << '\n';

        for (int i = 0; i < n; i++)
        {
            int ind_sum = tree_seg.r_get_sum(0, compr_vect[i][1] - 1, 0, (c / 2) - 1, 0);
            //fprintf(stderr, "i % 2 == 0 ind_sum = %d, sum = %d, parent = %d\n", ind_sum, tree_seg.tree[ind_sum].r_value, tree_seg.tree[ind_sum].r_parent);
            if ((ind_sum != -1) && (tree_seg.tree[ind_sum].r_value != 0)) tree_seg.r_parent[i] = tree_seg.tree[ind_sum].r_parent;

            int sum_to_up = 0;
            if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].r_value;
            //fprintf(stderr, "i % 2 == 0 val = %d, ind_to_update = %d, who = %d\n", sum_to_up + 1, (int)(c / 2) - 1  + compr_vect[i][1], i);
            tree_seg.l_update(sum_to_up + 1, (int) (c / 2) - 1 + compr_vect[i][1], i);

            ind_sum = tree_seg.l_get_sum(compr_vect[i][1] + 1, (c / 2) - 1, 0, (c / 2) - 1, 0);
            //fprintf(stderr, "i % 2 == 1 ind_sum = %d, sum = %d, parent = %d\n", ind_sum, tree_seg.tree[ind_sum].l_value, tree_seg.tree[ind_sum].l_parent);
            if ((ind_sum != -1) && (tree_seg.tree[ind_sum].l_value != 0)) tree_seg.l_parent[i] = tree_seg.tree[ind_sum].l_parent;

            sum_to_up = 0;
            if(ind_sum != -1) sum_to_up = tree_seg.tree[ind_sum].l_value;
            //fprintf(stderr, "i % 2 == 1 val = %d, ind_to_update = %d, who = %d\n", sum_to_up + 1, (int)(c / 2) - 1  + compr_vect[i][1], i);
            tree_seg.r_update(sum_to_up + 1, (int) (c / 2) - 1 + compr_vect[i][1], i);
        }

        std::cout << std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value)  << '\n';

        std::vector<int> way(std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value), 0);

        if(tree_seg.tree[0].l_value > tree_seg.tree[0].r_value) {
            FindRightWay(way, tree_seg, 1);
        }
        else{
            FindRightWay(way, tree_seg, 0);
        }

        std::cout << vect2[way[0]][1] << ' ';
        for (int i = 1; i < std::max(tree_seg.tree[0].l_value, tree_seg.tree[0].r_value); i++) {
            std::cout << vect2[way[i]][1] << ' ';
            assert(way[i] != 0);
        }
    }
}
