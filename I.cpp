#include<iostream>
#include<vector>
#include<bits/stdc++.h>


#include <stdint.h>
#include <stdlib.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>


struct Matrix {
  int m_size;
  long long mod;
  std::vector <std::vector<long long>> data;

  Matrix() = default;

  Matrix operator*(Matrix matrix) {
    Matrix new_matrix = *this;
    for (int i = 0; i < data.size(); i++) {
      for (int j = 0; j < data.size(); j++) {
        new_matrix.data[i][j] = 0;
        for (int k = 0; k < data.size(); k++)
          new_matrix.data[i][j] += (data[i][k] * matrix.data[k][j]) % mod;
        new_matrix.data[i][j] %= mod;
      }
    }
    return new_matrix;
  }

  Matrix MatrixDegreeInt(int degree) {
    if(degree == 0){
      Matrix res = *this;
      for (int i = 0; i < res.data.size(); i++) {
        for (int j = 0; j < res.data.size(); j++) {
          res.data[i][j] = (i == j ? 1 : 0);
        }
      }
      return res;
    }

    if(degree == 1) return *this;

    if(degree % 2 == 0){
      degree /= 2;
      Matrix matrix = MatrixDegreeInt(degree);
      return (matrix * matrix);
    }
    else{
      return (*this * MatrixDegreeInt(degree - 1));
    }
  }

  Matrix MatrixDegree2(std::string& degree){
    if(degree.size() == 0) return MatrixDegreeInt(0);
    if(degree.size() == 1){
      char meows = degree.back();
      degree.pop_back();
      int deg = meows - '0';
      return MatrixDegreeInt(deg);
    }
    char meows = degree.back();
    degree.pop_back();
    int deg = meows - '0';

    Matrix res_matrix = MatrixDegree2(degree);
    res_matrix = res_matrix.MatrixDegreeInt(10);
    Matrix res_matrix2 = MatrixDegreeInt(deg);
    return (res_matrix * res_matrix2);
  }
};




int main(){
  int n, n1, m1, mod;
  std::string m;
  std::cin >> m;
  std::cin >> n;
  std::cin >> mod;

  long long int max_dp = (1 << n);
  std::vector<std::vector<long long>> matrix(max_dp, std::vector<long long>(max_dp, 0));
  std::vector<std::vector<long long>> ed(max_dp, std::vector<long long>(max_dp, 0));

  for(int i = 0; i < max_dp; i++) {
    for(int j = 0; j < max_dp; j++) {
      if(i == j)  ed[i][j] = 1;
      int first  = i;
      int second = j;
      int pred_flag = -1;
      int cur_flag ;
      bool meow = true;
      for(int sdvig = 0; sdvig < n; sdvig++){
        if((first & 1) == (second & 1)) {
          cur_flag = first & 1;
          if(cur_flag == pred_flag) {
            meow = false;
            break;
          }
          else {
            pred_flag = cur_flag;
          }
        }
        else{
          pred_flag = -1;
        }
        second = second >> 1;
        first  = first  >> 1;
      }
      if(meow){
        matrix[i][j] = 1;
      }
    }
  }
  Matrix res;
  res.mod = mod;
  res.data = matrix;

  int me = m.size() - 1;
  while(m[me] == '0'){
    me--;
  }
  m[me] = '0' + (m[me] - '0' - 1);
  for(int i = me + 1; i < m.size(); i++){
    m[i] = '9';
  }
  res = res.MatrixDegree2(m);
  int sum = 0;
  for(int i = 0; i < max_dp; i++){
    for(int j = 0; j < max_dp; j++) {
      sum += res.data[i][j];
      sum %= mod;
    }
  }
  std::cout << sum;
}
