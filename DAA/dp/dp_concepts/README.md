# Dynamic Programming

## DP Trivial Concepts
### Finding a minimal element in an array of integers
```
min_elem(A[0,...,n-1] = min{ A[0], min_elem(A[1,...,n-1]) })

```
### **Longest Increasing Subsequence**
```cpp
int longest_increasing(const std::vector<int>& arr) {
    if(!arr.size()) {
        return 0;
    }
    std::vector<int> lis(arr.size(), 1);
    int max_incr{ 1 };
    for(int i = arr.size() - 2; i >= 0 ;--i) {
        int curr{ 0 };
        for(int j = i + 1; j < arr.size(); ++j) {
            if(arr[i] <= arr[j] && curr < lis[j]) {
                curr = lis[j];
            }
        }
        lis[i] += curr;
        if(max_incr < lis[i]) {
            max_incr = lis[i];
        }
    }
    return max_incr;
}
```   
* With **reconstruction** of sequence:
```cpp
struct res {
    int len{ 0 };
    int begin{ -1 };
    std::vector<int> parents;

    std::vector<int> get_sequence() const {
        std::vector<int> seq;
        int p = begin;
        while(p != -1) {
            seq.push_back(p);
            p = parents[p];
        }
        return std::move(seq);
    }
    bool is_ok() const {
        return len > 0 &&
               begin >= 0;
    }
};


res longest_increasing_reconstruct(const std::vector<int>& arr) {
    if(!arr.size()) {
        return {  };
    }
    std::vector<int> lis(arr.size(), 1);
    std::vector<int> parents(arr.size(), -1);

    int max_incr{ 1 };
    int begin = arr.size() - 1;

    for(int i = arr.size() - 2; i >= 0 ;--i) {
        int curr{ 0 };
        for(int j = i + 1; j < arr.size(); ++j) {
            if(arr[i] < arr[j] && curr < lis[j]) {
                curr = lis[j];
                parents[i] = j;
            }
        }
        lis[i] += curr;
        if(max_incr < lis[i]) {
            max_incr = lis[i];
            begin = i;
        }
    }
    return { max_incr, begin, std::move(parents) };
}

```  
### **Max sum subarray** *a.k.a.* Kadane's Algorithm(***plain old DP***)
```cpp
long long kadane(const std::vector<int>& A) {
    const int size = A.size();
    if(!size) {
        return INT_MIN;
    }
    long long starting_at = A[size - 1];
    long long max_so_far = starting_at;
    for(int i = size - 2; i >= 0; --i) {
            starting_at = starting_at > 0 ?
                          starting_at + A[i] : A[i];
            if(starting_at > max_so_far) {
                max_so_far = starting_at;
            }
    }
    return max_so_far;
}
```


