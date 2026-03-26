# Financial Portfolio Simulator (OOP Project)

This is a C++ investment portfolio simulator that allows for the management and evaluation of various types of financial instruments.

## Features
- **Asset Management:** Add and sell stocks, bonds, and bank deposits.
- **Time-based Evaluation:** Calculate the current value of the portfolio based on a simulated date.
- **Category-based Totals:** Use templates to find the total value invested in a specific type of asset.
- **Interactive Menu:** Easy-to-use console interface.

## Applied OOP Concepts
- **Inheritance & Polymorphism:** Base class `InstrumentFinanciar` and derived classes (`Stock`, `Bond`, `DepozitBancar`).
- **Pure Virtual Methods:** For dynamic value calculation and detail display.
- **Templates:** Generic method for filtering and processing asset categories.
- **Operator Overloading:** Overloaded `<<` operator for displaying dates and objects.

## How to Run
1. Download the `.cpp` file.
2. Compile it using any C++ development environment (e.g., Visual Studio, Code::Blocks).
3. Run the executable and follow the menu instructions.
