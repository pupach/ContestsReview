#include<iostream>
#include<vector>
#include<array>
#include<stack>

#define MAX_CAPACITY 1000000000000000



struct Edge{
  long long int numb_begin = 0;
  long long int numb_end = 0;
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
 private:
  std::deque<Top> tops;
  long long int count_top;
  long long int count_edg;
  std::deque<long long int> container_bfs = {};
  std::vector<Edge *> begin_edges  = {};
  std::vector<long long int> dp;

 public:
  static Graph ReadGraph(long long int amount_vertex, long long int amount_edge){
    Graph to_ret(amount_vertex, amount_edge);
    to_ret.ReadTopsFlowFromCIn();
    for (long long int i = 0; i < to_ret.count_edg; i++) {
      long long int f, s;
      std::cin >> f >> s;
      Edge* meow = to_ret.AddEdge(f, s);
      Edge* meow_ob = to_ret.AddEdge(s, f, 0);
      meow->ob = meow_ob;
      meow_ob->ob = meow;
    }
    to_ret.CreateSourceAndStock();
    
    return to_ret;
  }

  void CreateSourceAndStock() {
    tops[0].flow = MAX_CAPACITY;
    tops[count_top - 1].flow = MAX_CAPACITY;

    for (long long int i = 1; i < (count_top - 1); i++) {
      AddEdge(0, i, 0);
      AddEdge(i, count_top - 1, 0);
    }
  }

  Graph(long long int amount_vertex, long long int amount_edge=0) : count_top(amount_vertex + 2), count_edg(amount_edge), begin_edges(amount_edge, nullptr), tops(amount_vertex + 2), dp(amount_vertex + 2, 0) {}

  Edge * AddEdge(long long int numb_begin, long long int numb_end, long long int cap=MAX_CAPACITY) {
    Edge *meow = new Edge;
    meow->numb_begin = numb_begin;
    meow->numb_end = numb_end;
    meow->cap = cap;
    meow->ost_cap = cap;
    meow->color = 0;

    tops[numb_begin].edges_to.push_back(meow);
    return meow;
  }

  bool BFS() {
    container_bfs = std::deque<long long int>();
    dp = std::vector<long long int>(count_top, -1);
    long long int cur_top;
    container_bfs.push_back(0);
    dp[0] = 0;
    while(!container_bfs.empty()) {
      cur_top = container_bfs.front();
      container_bfs.pop_front();
      for(Edge* i : tops[cur_top].edges_to){
        if((i->ost_cap > 0) && (dp[i->numb_end] == -1)){
          dp[i->numb_end] = dp[cur_top] + 1;
          if(i->numb_end == (count_top - 1)) {
            return true;
          }
          container_bfs.push_back(i->numb_end);
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
        if(dp[(*meow)->numb_end] == dp[cur_top] + 1){
          long long int delta = DFS((*meow)->numb_end, std::min(able_capacity, (*meow)->ost_cap), color);
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
    while(BFS()) {
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

  const std::deque<Top>& GetTops() {
    return tops;
  }

  void ReadTopsFlowFromCIn() {
    long long int flow, max_flow = 0;
    for(std::size_t i = 1; i < count_top + 1; i++){
      std::cin >> flow;
      max_flow = std::max(max_flow, flow);
      tops[i].flow = flow;
    }
  }

};

int main() {
  long long int amount_vertex, amount_edge, max_flow = 0, sum = 0;
  std::cin >> amount_vertex >> amount_edge;

  Graph meow = Graph::ReadGraph(amount_vertex, amount_edge);

  for(std::size_t i = 1; i < amount_vertex + 1; i++){
    sum += meow.GetTops()[i].flow;
  }

  max_flow = meow.BinSearchMaxFlow(0, max_flow + 1, sum);
  std::cout << max_flow << '\n';
}
