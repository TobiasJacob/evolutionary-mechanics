# Evolutionary Mechanics

Following the [Project Description](latex/sources/ProjectDescription.md)

## Todo

- Use catch2 as testing framework
- Raffaele implements evolutionary algorithm
- Tobi implments the performance evaluator
- Tobi: Make heavier use of move semantics for vector calculations
- Tobi: Make sure that in equation checker all planes are connected, and not two disting bodies can emerge.
- Create a overload for const Matrix::Value, so that it can be used as a const reference in if type is const Matrix.
- Create a Sparse Matrix implementation / class
- Use a floodfilling algorithm to make sure that the planes are connected

## Rules

- Public variables / methods use PascalCase, private variables / methods use camelCase.
- Standard library headers are imported before local imports.

## Timeline

- `Nov 22 2020`: **Progress report**, single core version should be done
- `Nov 29 2020`: Multi core version should be done, just documentation left
- `Dec 6 2020`: **Deadline**
