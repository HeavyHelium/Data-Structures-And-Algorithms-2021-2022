#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

/// 0 -> 0, 1
/// 1 -> 1, 2
///

inline int adj1(const int i) { 
	return i;
}

inline int adj2(const int i) { 
	return adj1(i) + 1;
}


int max_path_sum(int root_row, 
		 int root_id, 
		 const std::vector<std::vector<int>>& triangle) { 
	
	int res = triangle[root_row][root_id];
	
	if(root_row + 1 >= triangle.size()) {
		return res; // no next row, no children
	}

	int left = 0;
	int right = 0;

	if(adj1(root_id) < triangle[root_row + 1].size()) { 
		left = max_path_sum(root_row + 1, 
				    adj1(root_id), 
				    triangle);
	}
	if(adj2(root_id) < triangle[root_row + 1].size()) { 
		right = max_path_sum(root_row + 1, 
				     adj2(root_id), 
				     triangle);
	}
	return res += std::max(left, right); 
}

int main() { 
	std::vector<std::vector<int>> triangle{ std::vector<int>{    1    }, 
       						std::vector<int>{  2 , 3  }, 
						std::vector<int>{ 1, 5, 1 }, };

	std::cout << "Max path sum is: "
	       	  << max_path_sum(0, 0, triangle) 
	          << "."  << std::endl;
	

	return 0;
}

