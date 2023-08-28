Set 1
=====

* There are 2 possible buggy functions

* deleteSet() 
* This function doesn't free the memory properly, which may cause memory leak

* validateMemUse() 
* This function incorrectly validate the memory

* Other functions should be working as intended


Set 2
=====

* There are 3 possible buggy functions

* insertItem()
* This function allows insertion of duplicate items while it should not

* removeItem(), 
* This function returns false when trying to remove an available item
* and returns true when trying to remove an unavailable item

* symmetricDifferenceOf()
* Given set1 = { 1, 2, 3, 4 } and set2 = { 2, 4, 5, 6 }
* The resulting set is not { 1, 3, 5, 6 }.
* This is either the result from the above buggy functions or this function is also buggy

* Other functions should be working as intended


Set 3
=====

* There are 2 possible buggy functions

* areDisjoint()
* The given set1 = { 1, 2, 3} and set 3 = { 4, 5, 6 , 7, 8 } are clearly disjoint
* but the function returns false, thus falty comparison

* symmetricDifferenceOf()
* Given set1 = { 1, 2, 3, 4 } and set2 = { 2, 4, 5, 6 }
* The resulting set is not { 1, 3, 5, 6 }.

* Other functions should be working as intended


Set 4
=====

* There is no failed test
* All functions should be working as intended from the test


Set 5
=====

* There are 4 possible buggy functions

* areEqual()
* The given set1 = { 1, 2, 3 } and set 2 = { 1, 2, 3 } are clearly equal
* but the function returns false, thus falty comparison

* areDisjoint()
* The given set1 = { 1, 2, 3} and set 3 = { 4, 5, 6 , 7, 8 } are clearly disjoint
* but the function returns false, thus falty comparison

* symmetricDifferenceOf()
* Given set1 = { 1, 2, 3, 4 } and set2 = { 2, 4, 5, 6 }
* The resulting set is not { 1, 2, 3, 4, 5, 6 }.


* symmetricDifferenceOf()
* Given set1 = { 1, 2, 3, 4 } and set2 = { 2, 4, 5, 6 }
* The resulting set is not { 1, 3, 5, 6 }.

* Other functions should be working as intended