#include<iostream>
#include<vector>
#include<stack>
#include<set>

struct Top {
  int color = 0;
  std::set<int> edges_to = {};
  bool is_comp_strong = false;
  int numb_comp_strong = -1;
};


class Graph{
 public:
  std::vector<Top> tops;
  int count_top;
  int count_edg;
  bool cycles = false;
  std::set<int> set = {};
  std::stack<int> way = {};
  std::stack<int> way_to_ret = {};
  std::vector<std::stack<int>> strong_connect = {};

  Graph(int n, int m=0) : count_top(n), count_edg(m), tops(n){}


  void GrapCinMatrix(int m = -1) {
    if (m != -1) count_edg = m;
    for (int i = 0; i < count_edg; i++) {
      int f, s;
      std::cin >> f >> s;
      tops[f - 1].edges_to.insert(s - 1);
      tops[s - 1].edges_to.insert(f - 1);
    }
  }

  void GrapCinMatrixOrg(int m = -1) {
    if (m != -1) count_edg = m;
    for (int i = 0; i < count_edg; i++) {
      int f, s;
      std::cin >> f >> s;
      tops[f - 1].edges_to.insert(s - 1);
    }
  }

  int DFS(int cur_top) {
    if(cycles)  return 0;

    way.push(cur_top);
    set.insert(cur_top);
    tops[cur_top].color = 1;
    auto iterator_set = tops[cur_top].edges_to.begin();;
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, iterator_set++){
      if(tops[*iterator_set].color == 0){
        DFS(*iterator_set);
      }
    }
    way.pop();
    tops[cur_top].color = 2;
    return 0;
  }

  int FindStrongConnect(int cur_top, int *cnt){
    tops[cur_top].color = *cnt;
    (*cnt)++;
    way.push(cur_top);
    way_to_ret.push(cur_top);
    auto iterator_set = tops[cur_top].edges_to.begin();
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, iterator_set++){
      if(tops[*iterator_set].color == 0){
        FindStrongConnect(*iterator_set, cnt);
      }
      else if(!tops[*iterator_set].is_comp_strong){
        while(tops[way.top()].color > tops[*iterator_set].color){
          way.pop();
        }
      }
    }

    if(way.top() == cur_top){
      strong_connect.push_back({});
      while(way_to_ret.top() != cur_top){
        tops[way_to_ret.top()].numb_comp_strong = strong_connect.size() - 1;
        strong_connect[strong_connect.size() - 1].push(way_to_ret.top());
        way_to_ret.pop();
      }
      tops[way_to_ret.top()].numb_comp_strong = strong_connect.size() - 1;
      strong_connect[strong_connect.size() - 1].push(way.top());
      way_to_ret.pop();
      way.pop();
      return 0;
    }
    return 0;
  }

  void FellWhite(){
    for(int i = 0; i < count_top; i++) {
      tops[i].color = 0;
    }
  }

  Graph Reverse(){
    Graph new_graph(count_top);

    for(int i = 0; i < count_top; i++) {
      auto iterator_set = tops[i].edges_to.begin();
      for (int j = 0; j < tops[i].edges_to.size(); j++, iterator_set++) {
        new_graph.tops[*iterator_set].edges_to.insert(i);
      }
    }

    return new_graph;
  }

  void Casaero(){
    for(int i = 0; i < count_top; i++) {
      if(tops[i].color == 0) {
        DFS_with_cycles(i);
        way = {};
      }
    }

    Graph reverse = Reverse();
    std::stack<int> stack = way_to_ret;

    for(int i = 0; i < count_top; i++){
      if(reverse.tops[stack.top()].color == 0){
        reverse.DFS_with_cycles(stack.top());
        strong_connect.push_back({});
        int size_s = reverse.way_to_ret.size();
        for(int j = 0; j < size_s; j++) {
          tops[reverse.way_to_ret.top()].numb_comp_strong = strong_connect.size() - 1;
          strong_connect[strong_connect.size() - 1].push(reverse.way_to_ret.top());
          reverse.way_to_ret.pop();
        }
        reverse.way_to_ret = {};
        reverse.way = {};
      }
      stack.pop();
    }
  }

  int DFS_with_cycles(int cur_top) {

    way.push(cur_top);
    tops[cur_top].color = 1;
    auto iterator_set = tops[cur_top].edges_to.begin();;
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, iterator_set++){
      if(tops[*iterator_set].color == 0){
        DFS_with_cycles(*iterator_set);
      }
      else if((!cycles) && (tops[*iterator_set].color == 1)){
        cycles = true;

      }

    }
    way_to_ret.push(way.top());
    way.pop();
    tops[cur_top].color = 2;
    return 0;
  }

  void MakeCondexGraph(Graph& condex_graph){
    for(int i = 0; i < count_top; i++){
      auto iterator_set = tops[i].edges_to.begin();
      for(int j = 0; j < tops[i].edges_to.size(); j++, iterator_set++){
        if(tops[*iterator_set].numb_comp_strong != tops[i].numb_comp_strong){
          condex_graph.tops[tops[i].numb_comp_strong].edges_to.insert(tops[*iterator_set].numb_comp_strong);
        }
      }
    }
  }
};



int main(){
  int n, m;
  std::cin >> n >> m;
  Graph graph(n, m);
  graph.GrapCinMatrixOrg();

  graph.Casaero();

  Graph condex_graph(graph.strong_connect.size());


  graph.MakeCondexGraph(condex_graph);


  for(int i = 0; i < condex_graph.tops.size(); i++) {
    if(condex_graph.tops[i].color == 0) {
      condex_graph.DFS_with_cycles(i);
      condex_graph.way = {};
    }
  }

  int size_s = condex_graph.way_to_ret.size();
  for (int i = 0; i < size_s; i++) {
    condex_graph.tops[condex_graph.way_to_ret.top()].numb_comp_strong = i + 1;
    condex_graph.way_to_ret.pop();
  }

  std::cout << size_s << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << condex_graph.tops[graph.tops[i].numb_comp_strong].numb_comp_strong << ' ';
  }
};