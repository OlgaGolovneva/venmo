# Insight Challenge: Analyze Venmo's dataset

This is a solution to the ["Venmo" challenge by Insight Data Science](https://github.com/InsightDataScience/coding-challenge).

The solution is written in C++, and takes its input in the JSON format.

The basic problem is to output the *running median* of the vertices' degrees of the given graph. Since the considered graphs are large but sparse, we don't use the standard "two heaps" solution. Instead, we maintain the counting sort of vertices' degrees. To optimize the running time, we make use of several additional data structures. 

There are several technical details which, by the problem statement, must be taken into account. For example, although the input graph edges *might not be ordered by time*, we still must output the running median for specific time windows.

The solution passed all tests by Insight Data Science.
