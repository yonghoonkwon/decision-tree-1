#include <cstdio>
#include "cart/stats.h" // mode
#include "cart/util.h" // range, slice
#include "random_forest/forest.h"

Forest::Forest() { }
Forest::Forest(int n_trees, int n_features) { init(n_trees, n_features); }

void Forest::init(int n_trees, int n_features) {
  this->n_trees = n_trees;
  this->n_features = n_features;

  for (int i = 0; i < n_trees; ++i) trees.push_back(TreeNode());
}

void Forest::train(Matrix &m) {
  //printf("forest training %lu %d\n", trees.size(), n_trees);
  std::vector<int> all_columns = range(m.columns()-1);
  for (int i = 0; i < trees.size(); ++i) {
    TreeNode &tree = trees[i];
    random_shuffle(all_columns.begin(), all_columns.end());
    std::vector<int> sub_cols = slice(all_columns, 0, n_features); // 训练列数
    tree.train(m, sub_cols);
  }
}

int Forest::classify(std::vector<double> &row) {
  std::vector<double> votes;
  for (int i = 0; i < n_trees; ++i) {
    TreeNode &tree = trees[i];
    double vote = tree.classify(row);
    votes.push_back(vote);
  }
  return (int)mode(votes);
}
