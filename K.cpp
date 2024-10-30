#include<iostream>
#include<vector>
#include<bits/stdc++.h>
/*
struct Point{
 public:
  double x;
  double y;

  static int CmpLinesAndPoints(Point& line, Point& point){
    double res = line.x * point.x + line.y;
    return res > point.y;
  }
};

class ConvexHullTrick{
 public:
  std::vector<Point> point_crossing;
  std::vector<Point> lines;

  int BinSearch(double x, int l, int r){
    if(l == r)  return l;
    int middle = (l + r) / 2;
    if(middle >= point_crossing.size())

    if(point_crossing[middle].x < x)        return BinSearch(x, middle + 1, r);
    else if(point_crossing[middle].x == x)  return middle;
    else                                    return BinSearch(x, l, middle);
  }

  int GetMin(double x){
    int index = BinSearch(x, 0, point_crossing.size());
    if(index >= point_crossing.size())  return 0;
    return lines[index + 1].x * x + lines[index + 1].y;
  }

  void Add(Point line_to_add){
    int i = point_crossing.size() - 1;
    while((i >= 0) && (Point::CmpLinesAndPoints(line_to_add, point_crossing[i]) == 1)) {
      i--;
      point_crossing.pop_back();
      lines.pop_back();
    }
    if(i >= 0){
      double x_to_add = (lines[i + 1].y - line_to_add.y) / (line_to_add.x - lines[i + 1].x);
      double y_to_add = x_to_add * line_to_add.x + line_to_add.y;
      point_crossing.push_back({x_to_add, y_to_add});
      }
    lines.push_back(line_to_add);
  }
};*/

int main(){
  int n, k;
  std::cin >> n >> k;
  if(k == 0){
    std::cout << 0;
    return 0;
  }
  std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n + 1, 0));

  int div = (n - k) / k;
  int ost = n - k - div * k;
  int res = 0;
  for(int i = 0; i < ost; i++){
    res += (div + 1) * (div + 1);
  }
  for(int i = 0; i < k - ost; i++){
    res += div * div;
  }

  std::cout << res;
}
