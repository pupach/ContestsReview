#include<iostream>
#include<vector>
#include<stack>
#include<set>

const int MAX_TWO_DEGREE = 22;

struct Top {
  int color = 0;
  int t_in  = 0;
  int parent = -1;
  std::vector<std::pair<int, int>> edges_to = {};

  bool IsEdgVector(int numb){
    for(int i = 0; i < edges_to.size(); i++){
      if(edges_to[i].first == numb){
        return true;
      }
    }
    return false;
  }
};


class Graph{

  std::vector<Top> tops;
  int count_top;
  int count_edg;
  int time = 0;
  int deep = 0;
  int ans  = 0;
  int *cnt;
  std::vector<std::pair<int, int>> edg = {};
  std::vector<bool> is_bridges = {};
  std::vector<std::set<int>> top_bridges = {};
  std::vector<int> arr_in  = {};
  std::vector<int> arr_out = {};
  std::vector<int> arr_deep = {};
  std::vector<int> arr_deep_bridges = {};
  std::vector<std::vector<int>> dp = {};
  Graph* ToBuild;

   public:
    Graph* GraphDp;

  Graph(int n, int m=0) : count_top(n), arr_deep_bridges(n), arr_in(n, 0), arr_out(n, 0), arr_deep(n, 0), dp(n, std::vector(21, 0)), count_edg(m), tops(n), is_bridges(m, false), top_bridges(n, std::set<int>()) {}

  void AddEdge(int top, int second_top, int numb){
    edg.push_back({top, second_top});

    tops[top].edges_to.push_back({second_top, numb});
    tops[second_top].edges_to.push_back({top, numb});
  }

  void GrapCinMatrix(int m = -1) {
    int first_top, second_top;
    if (m != -1) count_edg = m;
    for (int i = 0; i < count_edg; i++) {
      std::cin >> first_top >> second_top;
      AddEdge(first_top - 1, second_top - 1, i);
    }
  }

  int GetAns(){
    return ans;
  }
  void FellWhite(){
    for(int i = 0; i < count_top; i++) {
      tops[i].color = 0;
    }
  }

  int DFSToBuildTree(int cur_top) {
    tops[cur_top].color = 1;
    auto meow = tops[cur_top].edges_to.begin();;
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, meow++){
      if(tops[(*meow).first].color == 0){
        ToBuild->tops[cur_top].edges_to.push_back(*meow);
        DFSToBuildTree((*meow).first);
      }
    }
    tops[cur_top].color = 2;
    return 0;
  }

  void DFSSetToIn(int cur_top ,int parent){
    tops[cur_top].color = *cnt;
    tops[cur_top].t_in = *cnt;
    (*cnt)++;
    auto meow = tops[cur_top].edges_to.begin();
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, meow++){
      if(tops[(*meow).first].color != 0) {
        if((*meow).first != parent) {
          tops[cur_top].color = std::min(tops[(*meow).first].color, tops[cur_top].color);
        }
      }
    }
    meow = tops[cur_top].edges_to.begin();
    for(int i = 0; i < tops[cur_top].edges_to.size(); i++, meow++){
      if(tops[(*meow).first].color == 0) {
        DFSSetToIn((*meow).first, cur_top);
        tops[cur_top].color = std::min(tops[(*meow).first].color, tops[cur_top].color);
      }
    }
  }
  void FindBridges(){
    Graph to_ret(count_top);
    ToBuild = &to_ret;
    DFSToBuildTree(0);
    int cnt = 1;
    this->cnt = &cnt;

    FellWhite();

    DFSSetToIn(0,  -1);

    for(int i = 0; i  < edg.size(); i++){
      if (to_ret.tops[edg[i].first].IsEdgVector(edg[i].second)) {
        if (tops[edg[i].second].color == tops[edg[i].second].t_in) {
          is_bridges[i] = true;
          ans++;
        }
      }
      else if (to_ret.tops[edg[i].second].IsEdgVector(edg[i].first)) {
        if (tops[edg[i].first].color == tops[edg[i].first].t_in) {
          is_bridges[i] = true;
          ans++;
        }
      }
    }
  }

  void Fill_dp(int main){
    if(GraphDp->tops[main].parent == -1) GraphDp->tops[main].parent = main;
    GraphDp->arr_in[main] = GraphDp->time++;
    GraphDp->arr_deep[main] = GraphDp->deep++;

    tops[main].color = 1;
    for (int i = 0; i < tops[main].edges_to.size(); i++) {
      if(tops[tops[main].edges_to[i].first].color == 0){
        GraphDp->tops[main].edges_to.push_back({tops[main].edges_to[i].first, -1});
        GraphDp->tops[tops[main].edges_to[i].first].edges_to.push_back({main, -1});
        GraphDp->arr_deep_bridges[tops[main].edges_to[i].first] = GraphDp->arr_deep_bridges[main];
        if(is_bridges[tops[main].edges_to[i].second]){
          GraphDp->arr_deep_bridges[tops[main].edges_to[i].first]++;
        }
        GraphDp->tops[tops[main].edges_to[i].first].parent = main;
        Fill_dp(tops[main].edges_to[i].first);
      }
    }
    tops[main].color = 2;
    GraphDp->deep--;
    GraphDp->arr_out[main] = GraphDp->time++;

    return;
  }

  bool IsLeader(int x, int y){
    if((arr_in[x] <= arr_in[y]) && (arr_out[x] >= arr_out[y]))  return true;

    return false;
  }

  void Fill_dp(){
    for(int i = 0; i < MAX_TWO_DEGREE; i++){
      for(int j = 0; j < count_top; j++){
        if(i == 0){
          dp[j][i] = tops[j].parent;
        }
        else{
          dp[j][i] = dp[dp[j][i - 1]][i - 1];
        }
      }
    }
  }

  int LSA_with_distance(int x, int y){
    if(IsLeader(x, y))   return arr_deep_bridges[x];
    if(IsLeader(y, x))  return arr_deep_bridges[y];

    int cur_top = x;

    for(int i = MAX_TWO_DEGREE; i >= 0; i--){
        if ((!IsLeader(dp[cur_top][i], y)) && (IsLeader(dp[cur_top][i + 1], y))) {
          cur_top = dp[cur_top][i];
        }
    }
    return arr_deep_bridges[tops[cur_top].parent];
  }
};


int main(){
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, m, f , ans_size;
  std::cin >> n >> m;

  Graph graph(n, m);
  std::cin >> f ;

  graph.GrapCinMatrix(m);


  graph.FindBridges();

  graph.FellWhite();

  Graph GraphDp(n, graph.GetAns());

  graph.GraphDp = &GraphDp;
  graph.Fill_dp(f - 1);
  GraphDp.Fill_dp();




  std::cin >> ans_size;

  std::vector<int> deep   (ans_size, 0);

  for(int i = 0; i < ans_size; i++){
    std::cin >> n >> m;
    deep[i] = GraphDp.LSA_with_distance(n - 1, m - 1);
  }

  for(int i = 0; i < deep.size(); i++){
    std::cout << deep[i] << '\n';
  }
};
