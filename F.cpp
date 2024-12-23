#include<iostream>
#include<vector>
#include<array>
#include<stack>

#include<iostream>
#include<vector>
#include<stack>
#include<set>
#include<array>

#define MAX_CAPACITY 1000000000000000



struct Edge{
  long long int from = 0;
  long long int where = 0;
  long long int cap = 0;
  long long int ost_cap = 0;
  long long int color = 0;
  Edge* ob = nullptr;
};

struct Top {
  long long int color = 0;
  long long int flow = 0;
  std::vector<Edge *> edges_to = {};
};


class Graph{
 public:
  std::deque<Top> tops;
  long long int count_top;
  long long int count_edg;
  std::deque<long long int> container_bfs = {};
  std::vector<Edge *> begin_edges  = {};
  std::vector<long long int> dp;

  Graph(long long int n, long long int m=0) : count_top(n + 2), count_edg(m), begin_edges(m, nullptr), tops(n + 2), dp(n + 2, 0) {}

  Edge * AddEdge(long long int from, long long int where, long long int cap=MAX_CAPACITY) {
    Edge *meow = new Edge;
    meow->from = from;
    meow->where = where;
    meow->cap = cap;
    meow->ost_cap = cap;
    meow->color = 0;

    tops[from].edges_to.push_back(meow);
    return meow;
  }

  void GrapCinMatrix(long long int m = -1) {
    if (m != -1) count_edg = m;
    for (long long int i = 0; i < count_edg; i++) {
      long long int f, s;
      std::cin >> f >> s;
      Edge* meow = AddEdge(f, s);
      Edge* meow_ob = AddEdge(s, f, 0);
      meow->ob = meow_ob;
      meow_ob->ob = meow;
    }
    tops[0].flow = MAX_CAPACITY;
    tops[count_top - 1].flow = MAX_CAPACITY;

    for (long long int i = 1; i < (count_top - 1); i++) {
      Edge* meow = AddEdge(0, i, 0);
      Edge* meow_ob = AddEdge(i, 0, 0);
      meow->ob = meow_ob;
      meow_ob->ob = meow;

      Edge* meow1 = AddEdge(i, count_top - 1, 0);
      Edge* meow_ob1 = AddEdge(count_top - 1, i, 0);
      meow1->ob = meow_ob1;
      meow_ob1->ob = meow1;
    }
  }

  bool FillDP() {
    container_bfs = std::deque<long long int>();
    dp = std::vector<long long int>(count_top, -1);
    long long int cur_top;
    container_bfs.push_back(0);
    dp[0] = 0;
    while(!container_bfs.empty()) {
      cur_top = container_bfs.front();
      container_bfs.pop_front();
      for(Edge* i : tops[cur_top].edges_to){
        if((i->ost_cap > 0) && (dp[i->where] == -1)){
          dp[i->where] = dp[cur_top] + 1;
          if(i->where == (count_top - 1)) {
            return true;
          }
          container_bfs.push_back(i->where);
        }
      }
    }
    return (dp[count_top - 1] != -1);
  }

  long long int DFS(long long int cur_top, long long int able_capacity, int color) {
    if(((cur_top == (tops.size() - 1) )|| (able_capacity == 0))){
      return able_capacity;
    }
    auto meow = tops[cur_top].edges_to.begin();
    for(long long int i = 0; i < tops[cur_top].edges_to.size(); i++, meow++){
      if((*meow)->color != color) {
        if(dp[(*meow)->where] == dp[cur_top] + 1){
          long long int delta = DFS((*meow)->where, std::min(able_capacity, (*meow)->ost_cap), color);
          if(delta != 0) {
            if((*meow)->ost_cap != MAX_CAPACITY) (*meow)->ost_cap -= delta;
            if((*meow)->ob != nullptr) (*meow)->ob->ost_cap += delta;
            return delta;
          }
        }
        (*meow)->color = color;
      }
    }
    return 0;
  }

  long long int findMaxFlow() {
    long long int max_flow = 0;
    long long int delta = -1;
    int color = 2;
    while(FillDP()) {
      color++;
      delta = DFS(0, MAX_CAPACITY, color);

      while(delta != 0){
        max_flow += delta;
        delta = DFS(0, MAX_CAPACITY, color);
      }
    }
    return max_flow;
  }

  long long int BinSearchMaxFlow(long long int min_flow, long long int max_flow, long long int sum) {
    int result = max_flow;


    while (min_flow <= max_flow) {
      int between = min_flow + (max_flow - min_flow) / 2;

      for(std::size_t i = 1; i < (count_top - 1); i++){
        tops[0].edges_to[i - 1]->ost_cap = tops[i].flow;
        tops[0].edges_to[i - 1]->cap = tops[i].flow;
      }
      for(std::size_t i = 1; i < (count_top - 1); i++){
        tops[i].edges_to[tops[i].edges_to.size() - 1]->ost_cap = between;
        tops[i].edges_to[tops[i].edges_to.size() - 1]->cap = between;
      }

      if (findMaxFlow() == sum) {
        result = between;
        max_flow = between - 1;
      } else {
        min_flow = between + 1;
      }
    }
    return result;
  }

};

int main() {
  long long int n, m, flow, max_flow = 0, sum = 0;
  std::cin >> n >> m;

  Graph meow(n, m);
  for(std::size_t i = 1; i < n + 1; i++){
    std::cin >> flow;
    max_flow = std::max(max_flow, flow);
    meow.tops[i].flow = flow;
  }
  meow.GrapCinMatrix();

  for(std::size_t i = 1; i < n + 1; i++){
    sum += meow.tops[i].flow;
  }


  max_flow = meow.BinSearchMaxFlow(0, max_flow + 1, sum);
  std::cout << max_flow << '\n';


}