# **Disjoint Sets**/ **UnionFind** structure
## ***Definition***
```
A data structure which supports 3 operations: 
	- make_set - creates n singletons
	- union - unites 2 existing sets
	- find_set - returns an id of the set a given element is in
```
## Time complexity
* **to build**
	* **θ(n + m * α(n))** *amortized*   
PS: **α(n)** because a path in a tree is of length of at most n - 1
* find query takes θ(α(n)), near θ(1)
* same goes for union query
* In any **conceivable** application of disjoint-set data structure, **α(n) <=  4**
## Some uses
* ***Kruskal***'s algorithm for **MST**
* **Finding cycle** in an **undirected** graph
  
**Union by rank** and **path compression** are required for it to run efficiently
