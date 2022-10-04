<p align="center">
<img src="misc/main.JPG" alt="drawing"/>
</p>



# Decimal library
### Implementation of decimal.h library in the C programming language.<br>
This library add the ability to work with the "decimal" type, which is not in the language standard.<br>

## Information
The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335.<br> The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors.<br>

The binary representation of a Decimal number consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the integer number and specify what portion of it is a decimal fraction.

## Main functions

### Arithmetic Operators:
- Addition **(+)**
- Subtraction **(-)**
- Multiplication **(*)**
- Division **(/)**
- Modulo **(Mod)**

### Comparison Operators:
- Less than **(<)**
- Less than or equal to **(<=)**
- Greater than **(>)**
- Greater than or equal to **(>=)**
- Equal to **(==)**
- Not equal to **(!=)**

### Convertors and parsers:
- From int
- From float
- To int
- To float

### Another functions:
- Rounds a specified Decimal number to the closest integer toward negative infinity.
- Rounds a decimal value to the nearest integer.
- Returns the integral digits of the specified Decimal. Any fractional digits are discarded.
- Returns the result of multiplying the specified Decimal value by negative one.
