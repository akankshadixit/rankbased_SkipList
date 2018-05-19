# Rankbased_SkipList

The Rank based Skiplist implemented here is an authenticated data structure. In this skip list the bottom or leaf nodes store the data while every node v above the bottom level (which has two pointers, namely rgt(v) and dwn(v) ) also stores a label f(v) that is a cryptographic hash and is computed using some collision-resistant hash function h (e.g., SHA-1 in practice) as a function of f(rgt(v)) and f(dwn(v)). 

Using this data structure, one can answer queries like “does 21 belong to the set represented with this skip list?” and also provide a proof that the given answer is correct. To be able to verify the proofs to these answers, just hold the label f(s) of the top leftmost node of the skip list.


## Commutative Hash
Given a collision resistant hash function h, the label f(v) of a node v of a rank-based authenticated skiplist is defined as follows. 

```
Case 0: v = null
        f(v) = 0 
Case 1: l(v) > 0
        f(v) = h(l(v), r(v), f(dwn(v)), f(rgt(v))) 
Case 2: l(v) = 0
        f(v) = h(l(v), r(v), x(v), f(rgt(v))) 

```

	
