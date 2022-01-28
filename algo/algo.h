#ifndef HEURISTIC_H_06191147
#define HEURISTIC_H_06191147 

#include<string>
#include<unordered_map>
#include <algorithm>
#include<vector>
#include "syds.h"

Syds adaptive_linear_search_inference(const std::vector<std::vector<int>>& neighbors, const std::vector<int>& threshold);

Syds adaptive_binary_search_inference(const std::vector<std::vector<int>>& neighbors, const std::vector<int>& threshold);

std::vector<int> count_k(const std::vector<std::vector<int>>& neighbors, const std::vector<int>& threshold, int num_permu);
#endif
