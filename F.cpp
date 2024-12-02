#include<iostream>
#include<vector>
#include<stack>
#include<set>


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
 public:
  std::vector<Top> tops;
  int count_top;
  int count_edg;
  int time = 0;
  int deep = 0;
  int ans  = 0;
  int *cnt;
  bool cycles = false;
  std::vector<std::pair<int, int>> edg = {};
  std::vector<bool> is_bridges = {};
  std::vector<std::set<int>> top_bridges = {};
  std::vector<int> arr_in  = {};
  std::vector<int> arr_out = {};
  std::vector<int> arr_deep = {};
  std::vector<int> arr_deep_bridges = {};
  std::vector<std::vector<int>> dp = {};
  Graph* meow;
  Graph* ToBuild;

  Graph(int n, int m=0) : count_top(n), arr_deep_bridges(n), arr_in(n, 0), arr_out(n, 0), arr_deep(n, 0), dp(n, std::vector(21, 0)), count_edg(m), tops(n), is_bridges(m, false), top_bridges(n, std::set<int>()) {}

  void GrapCinMatrix(int m = -1) {
    int f, s;
    if (m != -1) count_edg = m;
    for (int i = 0; i < count_edg; i++) {
      std::cin >> f >> s;
      edg.push_back({f - 1, s - 1});

      tops[f - 1].edges_to.push_back({s - 1, i});
      tops[s - 1].edges_to.push_back({f - 1, i});
    }
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
    if(meow->tops[main].parent == -1) meow->tops[main].parent = main;
    meow->arr_in[main] = meow->time++;
    meow->arr_deep[main] = meow->deep++;

    tops[main].color = 1;
    for (int i = 0; i < tops[main].edges_to.size(); i++) {
      if(tops[tops[main].edges_to[i].first].color == 0){
        meow->tops[main].edges_to.push_back({tops[main].edges_to[i].first, -1});
        meow->tops[tops[main].edges_to[i].first].edges_to.push_back({main, -1});
        meow->arr_deep_bridges[tops[main].edges_to[i].first] = meow->arr_deep_bridges[main];
        if(is_bridges[tops[main].edges_to[i].second]){
          meow->arr_deep_bridges[tops[main].edges_to[i].first]++;
        }
        meow->tops[tops[main].edges_to[i].first].parent = main;
        Fill_dp(tops[main].edges_to[i].first);
      }
    }
    tops[main].color = 2;
    meow->deep--;
    meow->arr_out[main] = meow->time++;

    return;
  }

  bool IsLeader(int x, int y){
    if((arr_in[x] <= arr_in[y]) && (arr_out[x] >= arr_out[y]))  return true;

    return false;
  }

  void Fill_dp(){
    for(int i = 0; i < 22; i++){
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

    for(int i = 20; i > -1; i--){
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
  int n, m, main, ans_size;
  std::cin >> n >> m;

  Graph graph(n, m);
  std::cin >> main;

  graph.GrapCinMatrix(m);


  graph.FindBridges();

  graph.FellWhite();

  Graph meow(n, graph.ans);

  graph.meow = &meow;
  graph.Fill_dp(main - 1);
  meow.Fill_dp();




  std::cin >> ans_size;

  std::vector<int> deep   (ans_size, 0);

  for(int i = 0; i < ans_size; i++){
    std::cin >> n >> m;
    deep[i] = meow.LSA_with_distance(n - 1, m - 1);
    //std::cout << ans[i] << '\n';
  }



  for(int i = 0; i < deep.size(); i++){
    std::cout << deep[i] << '\n';
  }
};
