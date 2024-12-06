#include<iostream>
#include<vector>


class Matrix {
 public:
  long long mod;
  std::vector <std::vector<long long>> data;

  Matrix(int len_a = 0, int len_b = 0, long long mod = 0) : data(len_a, std::vector<long long>(len_b, 0)), mod(mod) {}

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

  Matrix MatrixPower(int power) {
    if(power == 0){
      Matrix res = *this;
      for (int i = 0; i < res.data.size(); i++) {
        for (int j = 0; j < res.data.size(); j++) {
          res.data[i][j] = (i == j ? 1 : 0);
        }
      }
      return res;
    }

    if(power == 1) return *this;

    if(power % 2 == 0){
      power /= 2;
      Matrix matrix = MatrixPower(power);
      return (matrix * matrix);
    }
    else{
      return (*this * MatrixPower(power - 1));
    }
  }

  Matrix Matrixpower2(std::string& power){
    if(power.size() == 0) return MatrixPower(0);

    if(power.size() == 1){
      char numb = power.back();
      power.pop_back();
      int deg = numb - '0';
      return MatrixPower(deg);
    }
    char numb = power.back();
    power.pop_back();
    int deg = numb - '0';

    Matrix res_matrix = Matrixpower2(power);
    res_matrix = res_matrix.MatrixPower(10);
    Matrix res_matrix2 = MatrixPower(deg);
    return (res_matrix * res_matrix2);
  }

  int MakeSumByMod(long long int max_dp){
    int sum = 0;
    for(int i = 0; i < max_dp; i++){
      for(int j = 0; j < max_dp; j++) {
        sum += data[i][j];
        sum %= mod;
      }
    }
    return sum;
  }
};


void FillTransitionalMatrix(Matrix& transition_matrix, long long int max_dp, int n){
  for(int i = 0; i < max_dp; i++) {
    for(int j = 0; j < max_dp; j++) {
      int first  = i;
      int second = j;
      int pred_flag = -1;
      int cur_flag ;
      bool flag = true;
      for(int shift = 0; shift < n; shift++){
        if((first & 1) == (second & 1)) {
          cur_flag = first & 1;
          if(cur_flag == pred_flag) {
            flag = false;
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
      if(flag){
        transition_matrix.data[i][j] = 1;
      }
    }
  }
}

void ParseString(std::string& m){
  int me = m.size() - 1;
  while(m[me] == '0'){
    me--;
  }
  m[me] = '0' + (m[me] - '0' - 1);
  for(int i = me + 1; i < m.size(); i++){
    m[i] = '9';
  }
}

int main(){
  int n, mod;
  std::string m;
  std::cin >> m;
  std::cin >> n;
  std::cin >> mod;

  long long int max_dp = (1 << n);
  Matrix transition_matrix(max_dp, max_dp, mod);
  Matrix ed_matrix        (max_dp, max_dp, mod);
  std::vector<std::vector<long long>> matrix(max_dp, std::vector<long long>(max_dp, 0));

  FillTransitionalMatrix(transition_matrix, max_dp, n);
  ParseString(m);
  transition_matrix = transition_matrix.Matrixpower2(m);
  int sum = transition_matrix.MakeSumByMod(max_dp);

  std::cout << sum;
}
