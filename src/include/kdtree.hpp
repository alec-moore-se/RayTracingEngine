#pragma once

#include <array>
#include <cstddef>
#include <iostream>

// simple kdtree implementation
// put here for future ideas

template <size_t K> class KDtree {
  typedef struct Node {
    std::array<double, K> cur;
    struct Node *left, *right;
    Node(std::array<double, K> cur) : cur(cur), left(nullptr), right(nullptr) {}
  } Node;

  Node *root;

  Node *newNodeRec(Node *node, const std::array<double, K> &p, int depth) {
    if (node == nullptr) {
      return new Node(p);
    }

    int cd = depth % K;
    if (p[cd] < node->cur[cd]) {
      node->left = newNodeRec(node->left, p, depth + 1);
    } else {
      node->right = newNodeRec(node->right, p, depth + 1);
    }
    return node;
  }

  bool searchRec(Node *node, const std::array<double, K> &p, int depth) {
    if (node == nullptr) {
      return false;
    }

    if (node->cur == p)
      return true;

    int cd = depth % K;
    if (p[cd] < node->cur[cd]) {
      return searchRec(node->left, p, depth + 1);
    } else {
      return searchRec(node->right, p, depth + 1);
    }
  }

  void printRec(Node *node, int depth) const {
    if (node == nullptr)
      return;
    for (int i = 0; i < depth; i++) {
      std::cout << "  ";
    }
    std::cout << '(';
    for (size_t i = 0; i < K; i++) {
      std::cout << node->cur[i];
      if (i < K - 1)
        std::cout << ", ";
    }
    std::cout << ")\n";
    printRec(node->left, depth + 1);
    printRec(node->right, depth + 1);
  }

public:
  KDtree() { root = nullptr; }
  KDtree(const std::array<double, K> &p) { root = newNodeRec(nullptr, p, 0); }
  ~KDtree() { delete root; }

  bool search(const std::array<double, K> &p) const {
    return searchRec(root, p, 0);
  }

  void insert(const std::array<double, K> &p) { root = newNodeRec(root, p, 0); }

  void print() const { printRec(root, 0); }
};
