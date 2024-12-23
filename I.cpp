#include<iostream>
#include<vector>


class BigInt{
 public:
  std::string str_int;
  
  BigInt(const std::string& numb) : str_int(numb) {}

  void DecrementNumbInString(){
    int me = str_int.size() - 1;
    while(str_int[me] == '0'){
      me--;
    }
    str_int[me] = '0' + (str_int[me] - '0' - 1);
    for(int i = me + 1; i < str_int.size(); i++){
      str_int[i] = '9';
    }
  }

  void OpDivideTen(){
    str_int.pop_back();
  }
  int OpRemainsTen(){
    return str_int.back() - '0';
  }
};

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

  Matrix MatrixpowerBigInt(BigInt& power){
    power.DecrementNumbInString();

    if(power.str_int.size() == 0){
      int deg = power.OpRemainsTen();
      power.OpDivideTen();
      return MatrixPower(deg);
    }
    int deg = power.OpRemainsTen();
    power.OpDivideTen();

    Matrix res_matrix = MatrixpowerBigInt(power);
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

bool EqualModTwo(int isFirstEven, int isSecondEven) {
  return ((isFirstEven % 2) == (isSecondEven % 2));
}


Matrix FillTransitionalMatrix(long long int max_dp, int n){
  Matrix transition_matrix(max_dp, max_dp, 5);

  for(int i = 0; i < max_dp; i++) {
    for(int j = 0; j < max_dp; j++) {
      int first  = i;
      int second = j;
      int pred_flag = -1;
      int cur_flag ;
      bool flag = true;
      for(int shift = 0; shift < n; shift++){
        if(EqualModTwo(first, second)) {
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
  return transition_matrix;
}


int main(){
  int size, mod;
  std::string string_power;
  std::cin >> string_power;
  std::cin >> size;
  std::cin >> mod;

  long long int max_dp = (1 << size);
  Matrix ed_matrix        (max_dp, max_dp, mod);
  std::vector<std::vector<long long>> matrix(max_dp, std::vector<long long>(max_dp, 0));

  Matrix transition_matrix = FillTransitionalMatrix(max_dp, size);
  transition_matrix.mod = mod;
  BigInt power(string_power);
  transition_matrix = transition_matrix.MatrixpowerBigInt(power);
  int sum = transition_matrix.MakeSumByMod(max_dp);

  std::cout << sum;
}
