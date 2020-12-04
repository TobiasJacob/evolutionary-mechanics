# Evolutionary Mechanics

Following the [Project Description](latex/sources/ProjectDescription.md)

## Todo

- Use catch2 as testing framework
- Raffaele implements evolutionary algorithm
- Tobi: Make heavier use of move semantics for vector calculations
- Tobi: Make sure that in equation checker all planes are connected, and not two disting bodies can emerge.
- Tobi: Try out hashmap or BST instead of list container.
- Tobi: Check the indexes of supports and forces to make sure they are valid
- Create a overload for const Matrix::Value, so that it can be used as a const reference in if type is const Matrix.
- Create a Sparse Matrix implementation / class
- Use a floodfilling algorithm to make sure that the planes are connected

## Rules

- Public variables / methods use PascalCase, private variables / methods use camelCase.
- Standard library headers are imported before local imports.
- Remeber to include `#include "catch/catch.hpp"` on your test files

## Timeline

- `Nov 22 2020`: **Progress report**, single core version should be done
- `Nov 29 2020`: Multi core version should be done, just documentation left
- `Dec 6 2020`: **Deadline**

## LOG

- `Nov 16 2020`: Added Catch2 testing framework

data backup
    \begin{table}[h]
        \centering
        \small
        \begin{tabular}{rrrrrr}
            $n$ & Equations & Steps for solution & Equation setup time & Reduction time & solving time \\
            - & $O(n^2)$ & $O(n)$ & $O(n^2)$ & $O(n^4)$ & $O(n^3)$\\
            200 & 80799 & 6555 & 98695 & 7454400 & 22344100 \\
            100 & 23187 & 2146 & 23187 & 425889 & 1726910 \\
            50 & 5199 & 1039 & 6203 & 278603 & 154818 \\
            25 & 1349 & 449 & 1626 & 20503 & 17768 \\
            12 & 335 & 210 & 361 & 1347 & 1786
        \end{tabular}
        \caption{Execution time for matrix solver}
        \label{tab:profiling}
    \end{table}


