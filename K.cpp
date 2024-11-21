#include<iostream>

int main(){
  int n, k;
  std::cin >> n >> k;
  if(k == 0){
    std::cout << 0;
    return 0;
  }

  int div = (n - k) / k;
  int remainder = n - k - div * k;
  int res = 0;
  for(int i = 0; i < remainder; i++){
    res += (div + 1) * (div + 1);
  }
  for(int i = 0; i < k - remainder; i++){
    res += div * div;
  }

  std::cout << res;
}
