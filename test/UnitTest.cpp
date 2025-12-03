#include "Lqmalloc.h"
#include "ObjectPool.hpp"

struct TreeNode {
  int _val;
  TreeNode* _left;
  TreeNode* _right;
  TreeNode() : _val(0), _left(nullptr), _right(nullptr) {}
};

void TestObjectPool() {
  const size_t Rounds = 3;  // 申请轮次
  const size_t N = 100000;  // 每轮申请次数

  std::vector<TreeNode*> v1;
  v1.reserve(N);

  size_t begin1 = clock();
  for (size_t j = 0; j < Rounds; ++j) {
    for (int i = 0; i < N; ++i) {
      v1.push_back(new TreeNode);
    }
    for (int i = 0; i < N; ++i) {
      delete v1[i];
    }
    v1.clear();
  }
  size_t end1 = clock();

  ObjectPool<TreeNode> TNPool;
  std::vector<TreeNode*> v2;
  v2.reserve(N);

  size_t begin2 = clock();
  for (size_t j = 0; j < Rounds; ++j) {
    for (int i = 0; i < N; ++i) {
      v2.push_back(TNPool.New());
    }
    for (int i = 0; i < N; ++i) {
      TNPool.Delete(v2[i]);
    }
    v2.clear();
  }
  size_t end2 = clock();

  cout << "new cost time:" << end1 - begin1 << endl;
  cout << "object pool cost time:" << end2 - begin2 << endl;
}

void Testlqmalloc1() {
  std::thread t1([]() {
    for (size_t i = 0; i < 5; ++i) {
      lqmalloc(6);
    }
  });

  std::thread t2([]() {
    for (size_t i = 0; i < 5; ++i) {
      lqmalloc(7);
    }
  });

  t1.join();
  t2.join();
}

void Testlqmalloc2() {
  void* p1 = lqmalloc(6);
  void* p2 = lqmalloc(7);
  void* p3 = lqmalloc(1);
  void* p4 = lqmalloc(3);

  lqfree(p1);
  lqfree(p2);
  lqfree(p3);
  lqfree(p4);
}

// int main() {
//   // TestObjectPool();
//   Testlqmalloc1();
//   return 0;
// }