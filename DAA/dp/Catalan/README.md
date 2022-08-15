# Catalan numbers
## Definition
* ```def``` Cn = C0 * Cn-1 + C1 * Cn-2 + C2 * Cn-3 + ... + Cn-1 * C0  
	    **C0, C1 = 1**
	* sum of indices must be n - 1
	* DP is obviously *very suitable* for calculation of **nth** Catalan number
* 1, 2, 5, 14, ***42***, 132, ...

## Associated problems  
Many counting problems map to Catlan number calculation.
* Number of BSTs
* Unbalanced trees
	* that's just n! * Cn since n! is the number of possible orderings 
* N parantheses

* Dyck Words
* Mountain Ranges
* Convex Polygon
* Connect Disjoint Chords
* Ways of Triangulation

*for more explanations:* [Keerti's video](https://www.youtube.com/watch?v=0pTN0qzpt-Y&ab_channel=KeertiPurswani)  
   
```Side note:``` Mind that Catalan numbers can be calculated in linear time. The implementation following the definition + DP reduces it only to quadratic.
