#  **Van Emde Boas Tree**

## About the data structure
* ***Goal***
  * store n integers from a *Universe* **u**
    
    * i.e. the Universe represents a range **[0, u)**
  
  * solution to the **predecessor/successor problem**

* **Uses**
  * **Routing tables** 
    
    * IP addresses, ranges, hence predecessor/successor problem arises
      
      * IPv4: **u** = 2<sup>32</sup>
  
  * ...more that I am oblivious of
  
## Operations
| **Operation** | **Complexity** |
|---------------|----------------|
| *Insert*      | O(log log u)   |
| *Delete*      | O(log log u)   |
| *Predecessor* | O(log log u)   |
| *Successor*   | O(log log u)   |

* ```Note:``` when u = n<sup>O(1)</sup> or u = n<sup>(log n)<sup>O(1)</sup></sup>, then **u = log log *n***
<br></br>
* Exponentially faster than balanced BSTs