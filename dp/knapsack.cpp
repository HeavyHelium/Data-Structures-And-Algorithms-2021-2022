#include <vector>
#include <algorithm>
#include <iostream>

int knapsack(int capacity, int weights[], int values[], int count)
{
   if(!count || !capacity) {
       return 0;
   }
   std::vector<std::vector<int>> dp(capacity + 1,
                                    std::vector<int>(count));
   for(int i = 0; i <= capacity; ++i) {
       if(weights[count - 1] <= i) {
           dp[i][count - 1] = values[count - 1];
       } else {
           dp[i][count - 1] = 0;
       }
   }
   for(int j = count - 2; j >= 0; --j) {
       for(int i = 0; i <= capacity; ++i) {
           int skip_value = dp[i][j + 1];
           int take_value = weights[j] <= i ?
                            values[j] + dp[i - weights[j]][j + 1] : 0;
            dp[i][j] = std::max(skip_value, take_value);
       }
   }
   return dp[capacity][0];
}


int main() {
    constexpr int capacity = 41;
    constexpr int count = 58;
    int weights[count] = { 57,95,13,29,1,99,34,77,61,23,24,70,
                           73,88,33,61,43,5,41,63,8,67,20,72,98,
                           59,46,58,64,94,97,70,46,81,42,7,1,52,
                           20,54,81,3,73,78,81,11,41,45,18,94,24,
                           82,9,19,59,48,2,72 };
    int values[count] = { 83,84,85,76,13,87,2,23,33,82,79,100,88,
                          85,91,78,83,44,4,50,11,68,90,88,73,83,46,
                          16,7,35,76,31,40,49,65,2,18,47,55,38,75,
                          58,86,77,96,94,82,92,10,86,54,49,65,44,77,
                          22,81,52 };
    std::cout << knapsack(capacity, weights, values, count) << std::endl;
    return 0;
}
