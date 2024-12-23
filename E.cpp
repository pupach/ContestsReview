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

  Graph(int n, int m=0) : count_top(n), count_edg(m), vertexs(n){}

  void OrgGrapCinFromMatrix(int m = -1) {
    if (m != -1) count_edg = m;
    for (int i = 0; i < count_edg; i++) {
      int f, s;
      std::cin >> f >> s;
      vertexs[f - 1].edges_to.insert(s - 1);
    }
  }
  
  void FindStrongConnect(){
    int size_s = way_to_ret.size();
    for (int i = 0; i < size_s; i++) {
      vertexs[way_to_ret.top()].numb_comp_strong = i + 1;
      way_to_ret.pop();
    }
  }
  
  std::size_t GetWayToRetSize(){
    return way_to_ret.size();
  }

  void FellWhite(){
    for(int i = 0; i < count_top; i++) {
      vertexs[i].color = 0;
    }
  }

  Graph Reverse(){
    Graph new_graph(count_top);

    for(int i = 0; i < count_top; i++) {
      auto iterator_set = vertexs[i].edges_to.begin();
      for (int j = 0; j < vertexs[i].edges_to.size(); j++, iterator_set++) {
        new_graph.vertexs[*iterator_set].edges_to.insert(i);
      }
    }

    return new_graph;
  }

  void Casaero(){
    for(int i = 0; i < count_top; i++) {
      if(vertexs[i].color == 0) {
        _DFS_with_cycles(i);
        buffer_vertex = {};
      }
    }

    Graph reverse = Reverse();
    std::stack<int> stack = way_to_ret;

    for(int i = 0; i < count_top; i++){
      if(reverse.vertexs[stack.top()].color == 0){
        reverse._DFS_with_cycles(stack.top());
        strong_connect.push_back({});
        int size_s = reverse.way_to_ret.size();
        for(int j = 0; j < size_s; j++) {
          vertexs[reverse.way_to_ret.top()].numb_comp_strong = strong_connect.size() - 1;
          strong_connect[strong_connect.size() - 1].push(reverse.way_to_ret.top());
          reverse.way_to_ret.pop();
        }
        reverse.way_to_ret = {};
        reverse.buffer_vertex = {};
      }
      stack.pop();
    }
  }
  
  void DFS_with_cycles() {
    buffer_vertex = {};
    for (int i = 0; i < vertexs.size(); i++) {
      if (vertexs[i].color == 0) {
        int ret = _DFS_with_cycles(i);
      }

      buffer_vertex = {};
    }
  }

  void MakeCondexGraph(Graph& condex_graph){
    for(int i = 0; i < count_top; i++){
      auto iterator_set = vertexs[i].edges_to.begin();
      for(int j = 0; j < vertexs[i].edges_to.size(); j++, iterator_set++){
        if(vertexs[*iterator_set].numb_comp_strong != vertexs[i].numb_comp_strong){
          condex_graph.vertexs[vertexs[i].numb_comp_strong].edges_to.insert(vertexs[*iterator_set].numb_comp_strong);
        }
      }
    }
  }
  const std::vector<std::stack<int>>& GetStrongConnect() {
    return strong_connect;
  }
  const std::vector<Top>& GetVetexs() {
    return vertexs;
  }
 private:

  int count_top;
  int count_edg;
  bool cycles = false;
  std::vector<Top> vertexs;
  std::stack<int> buffer_vertex = {};
  std::stack<int> way_to_ret = {};
  std::vector<std::stack<int>> strong_connect = {};

  int _DFS_with_cycles(int cur_top) {

    buffer_vertex.push(cur_top);
    vertexs[cur_top].color = 1;
    auto iterator_set = vertexs[cur_top].edges_to.begin();;
    for(int i = 0; i < vertexs[cur_top].edges_to.size(); i++, iterator_set++){
      if(vertexs[*iterator_set].color == 0){
        _DFS_with_cycles(*iterator_set);
      }
      else if((!cycles) && (vertexs[*iterator_set].color == 1)){
        cycles = true;
      }
    }
    way_to_ret.push(buffer_vertex.top());
    buffer_vertex.pop();
    vertexs[cur_top].color = 2;
    return 0;
  }
};

int main(){
  int n, m;
  std::cin >> n >> m;
  Graph graph(n, m);
  graph.OrgGrapCinFromMatrix();

  graph.Casaero();

  Graph condex_graph(graph.GetStrongConnect().size());


  graph.MakeCondexGraph(condex_graph);

  condex_graph.DFS_with_cycles();

  condex_graph.FindStrongConnect();

  std::cout << condex_graph.GetWayToRetSize() << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << condex_graph.GetVetexs()[graph.GetVetexs()[i].numb_comp_strong].numb_comp_strong << ' ';
  }
};
