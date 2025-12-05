# 🧩 Small Problem Solutions

This repository is a collection of solutions written in C to various small programming and algorithmic problems. Each problem is contained within its own file. Additionally, I've created my own data structures from scratch to solve these small problems. For more info about these data structures see [UTILS.md](https://github.com/ggarper1/SmallCProjects/blob/main/UTILS.md)

---

## 📝 Problem Index

| Problem File | Description |
| :--- | :--- |
| `droneOptimization` | Find the minimum flight time for two drones that deliver packages. |
| `smallestInListV1` | Find the smallest element in a list thay can be obtained after a certain amount of operations. Easy version. |
| `smallestInListV2` | Find the smallest element in a list thay can be obtained after a certain amount of operations. Medium version |
| `smallestInListV3` | Find the smallest element in a list thay can be obtained after a certain amount of operations. Harder version |
| `arraySumOperation` | Apply operations on a array and obtain the sum of it's elements after each one. |
| `getBTHeight` | Gets a binary tree's height only accesing the tree's root. |
| `bitwiseIntAnd` | Given two numbers, `k` and `n` find two different numbers, `a` and `b`, both not larger than `N`, such that `a AND b` is as large as possible but less than `k`. |

---

## 💡 Problem Explanations

### `droneOptimization`

**The Problem:** Two drones must deliver packages. The first drone must deliver `d1` packages, the second must deliver `d2` packages. Additionally, the first drone must charge at all hours multiple of `c1` and the second at all hours multiples of `c2`. Knowing that they cannot deliver packages at the same time, that all deliveries take 1 hour and all charges also take 1 hour, what's the minimum amount of hours all packages could be delivered in?

**The Solution:** The key to the solution is realizing that every hour at least one drone will be able to deliver a package except those hours where both drones need to charge.

### `closestSubsetSum`

**The Problem:** You are given a list of integers and you must find the sum of integers from that list that is most similar to any element from that list.

**The Solution:** Not solved yet


### `smallestInListV1`

**The Problem:** You are given a list of positive integers `l` and an integer `maxOperations` greater than 0. You must find the smallest element that can be achieved in at most `maxOperations` operations. There is only one operation you can perform: choose two elements from the list and append to it the absolute value of their difference. The results of these operations are appended to the list, meaning they can be reused in other operations and can also be considered when finding the smallest number in the list.

**The Solution:** The solution is straightforward if we can repeat operations (i.e., apply the operation on the same two numbers multiple times). The problem becomes harder if we cannot do this; see problem `smallestInListV2`.

### `smallestInListV2`

**The Problem:** You are given a list of positive integers `l` and an integer `maxOperations` greater than 0. You must find the smallest element that can be achieved in at most `maxOperations` operations. There is only one operation you can perform: choose two elements from the list and append to it the absolute value of their difference. The results of these operations are appended to the list, meaning they can be reused in other operations and can also be considered when finding the smallest number in the list. You cannot reuse numbers for operations; that is, if you have applied the operation to the items at positions 2 and 5, you cannot perform another operation using the numbers at positions 2 or 5.

**The Solution:** No solution yet.

**Example:**
Input:
- `l` = [10, 30, 34, 43]
- `maxOperations` = 3
Output: 1

### `smallestInListV3`

**The Problem:** You are given a list of positive integers `l` and an integer `maxOperations` greater than 0. You must find the smallest element that can be achieved in at most `maxOperations` operations. There is only one operation you can perform: choose two elements from the list and append to it the absolute value of their difference. The results of these operations are appended to the list, meaning they can be reused in other operations and can also be considered when finding the smallest number in the list. You cannot reuse the *same pair* of numbers for operations; that is, if you have applied the operation to the items at positions 2 and 5, you cannot perform another operation using the same pair (2, 5). However, you can perform an operation with one of them and a different number (e.g., positions 2 and 3).

**The Solution:** No solution yet.

**Example:**
Input:
- `l` = [10, 32, 34, 44]
- `maxOperations` = 3
Output: 0

### `arraySumOperation`

**The Problem:** Problem from HackerRank 'Hack the Interview VI (U.S.)' competition. You have an identity permutation of integers as an array initially. An identity permutation of integers for `N` is a list containing all strictly positive numbers up to `N`. On this problem, you have to perform operations on this array and report the sum of the elements of the array after each operation.

The i_th operation consists of an integer `opi`.

1. If the array contains `opi`, swap the first and last elements in the array.
2. Else, remove the last element of the array and push  to the end of the array.

The input is read from a file containing in the first line the values for `N` and `M` separated by a space. `M` is the number of operations. After that, line i_th contains the value for operation i_th.


### `getBTHeight`

**The Problem:** Obtain a tree's height only having access to the tree's root.


### `bitwiseIntAnd`

**The Problem:** Given two numbers, `k` and `n` find two different numbers, `a` and `b`, in the interval `[1,2,3,...,N]`, such that `a AND b` is as large as possible but less than `k`.
