#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

std::vector<uint64_t> nums;
std::vector<uint64_t> t;
std::vector<uint64_t> factorials;

template<typename T>
void Print(const std::vector<T>& arr) {
  for (const auto& el : arr) {
    std::cout << el << "\t";
  }
  std::cout << "\n";
}

void SetFactorials(uint64_t n) {
  const int64_t kMod = 1e9 + 7;
  factorials[0] = 1;
  for (uint64_t i = 1; i < n; i++) {
    factorials[i] = (factorials[i - 1] * i) % kMod;
  }
  std::reverse(factorials.begin(), factorials.end());
}

uint64_t Sum(uint64_t a, uint64_t b) {
  return a + b;
}

void Build(uint64_t vertex_num, uint64_t l, uint64_t r) {
  if (l == r - 1) {
    t[vertex_num] = nums[l];
    return;
  }
  uint64_t m = (l + r) / 2;
  Build(vertex_num * 2 + 1, l, m);
  Build(vertex_num * 2 + 2, m, r);
  t[vertex_num] = Sum(t[vertex_num * 2 + 1], t[vertex_num * 2 + 2]);
}

uint64_t GetSum(uint64_t vertex_num,
                uint64_t l,
                uint64_t r,
                uint64_t getl,
                uint64_t getr) {
  /*  getl, getr -- границы запроса */
  if (l >= getr || r <= getl) {
    return 0;
  }
  if (l >= getl && r <= getr) {
    return t[vertex_num];
  }
  uint64_t m = (l + r) / 2;
  return Sum(GetSum(vertex_num * 2 + 1, l, m, getl, getr),
             GetSum(vertex_num * 2 + 2, m, r, getl, getr));
}

void Change(uint64_t vertex_num,
            uint64_t l,
            uint64_t r,
            uint64_t position,
            uint64_t value) {
  if (l == r - 1) {
    t[vertex_num] = value;
    return;
  }
  uint64_t m = (l + r) / 2;
  if (position < m) {
    Change(vertex_num * 2 + 1, l, m, position, value);
  } else {
    Change(vertex_num * 2 + 2, m, r, position, value);
  }
  t[vertex_num] = Sum(t[vertex_num * 2 + 1], t[vertex_num * 2 + 2]);
}

int main() {
  const uint64_t kMod = 1e9 + 7;
  uint64_t n = 0;
  std::cin >> n;
  nums.resize(n, 1);
  t.resize(4 * n);
  Build(0, 0, n);

  std::vector<uint64_t> permutations(n);
  for (uint64_t i = 0; i < n; i++) {
    // permutations[i] = n - i;
    std::cin >> permutations[i];
  }

  factorials.resize(n);
  SetFactorials(n);

  uint64_t res = 0;
  for (uint64_t i = 0; i < n; i++) {
    res = (res + factorials[i] * GetSum(0, 0, n, 0, permutations[i] - 1)) % kMod;
    Change(0, 0, n, permutations[i] - 1, 0);
  }
  std::cout << ++res;
  return 0;
}
