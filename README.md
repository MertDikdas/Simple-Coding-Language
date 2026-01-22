# Simple Coding Language

This is a simple coding language interpreter for Plus++ language. It is builded for school project.

## Project Summary

- It is written with C language
- It has basic logic
- It has lexical analyzer and parser
- It is work with only files that has extension `.ppp` 

## Compile

To change and compile the project you need an C compiler like gcc

```bash
gcc ppp.c lexical_analyzer.c parser.c -o ppp
```

## Execution
```bash
ppp script_name
```
-script_name is your Plus++ source code

## Setup

```bash
git clone https://github.com/MertDikdas/Simple-Coding-Language.git
cd Simple-Coding-Language
```

## Features

It has basic syntax
A line of code is one of the following:
• A variable declaration.
• An assignment statement.
• An increment statement.
• A decrement statement.
• An output statement.
• A loop statement.
Comments:
Comments are written between asterix characters. Can cover multiple lines.
example: *this is a comment* * and
this is a comment too*
Data Types: The only data type for variables is the number. Number is a signed whole number that is represented as
a decimal data type. A number can be as big as 100 decimal digits.
Strings exist only as constants and are used in the write statements only.
example: 123113, -5, 0, -314159 are valid number representations.
3.14159 is not a valid number (it is a real number, i.e. float…)
3.0 is not a valid number (decimal point and fraction digit should not be displayed)
3. is not a valid number (decimal point should not be displayed)
- 5 is not a valid number (there should be no blank between the minus sign and the first digit.
--5 is not a valid number (only one minus sign allowed)
+5 is not a valid number (the unnecessary plus sign is not allowed)
Variables: All variables should be declared as a number. Variables must be declared before they are used. All variables
are global (and static).
int <variable>.
Variable names are case sensitive and have a maximum length of 20 characters. Variable names start with a letter
(alphabetical character) and continue with alphanumeric characters or an underscore character.
example: number myVar;
All variables are initialized with the value 0 at the time of creation.
Assignment Statement:
<variable> := <int_value> ;
example: myVar:= 25; *assigns 25 to myVar*
yourVar := myVar; *assigns myVar to yourVar*
Increment Statement: Increase the value of a number.
<variable> += <int_value> ;
Increments the variable by int_value.
example: sum+=2; *The value of sum increases by 2*
Decrement Statement: Decrease the value of a number.
<variable> -= <int_value>;
example: total-=t; *The value of total decreases by t*
Write Statement: Display on screen
write <output_list>; *List elements are separated by the keyword “and”*
The `write` statement is used to output values and strings to the standard output.
example: write “The result is:” and sum.
An integer value is either a variable or a constant.
<int_value>→<variable>|<int_const>
An output list is a list of strings and integer values separated by the “and” keyword.
<out_list>→<out_list>and<list_element>|<list_element>
<list_element>→<int_value>|<string>| newline
A string is any sequence of characters between two quotation marks.
example: “Hello, this is a string!”
The `newline` Keyword: The `newline` keyword, when used in a `write` statement's output list,
instructs the interpreter to advance the output cursor to the beginning of the next line. It does not
print any visible characters itself but controls the formatting of subsequent output.
example: write "First line." and newline and "Second line.";
*This would output: First line.
Second line. *
Loop:
repeat <int_value> times <line>
*OR*
repeat <int_value> times <code_block>
A loop starts with the int_value and at each iteration decrements the value of int_value by one. If
int_value is a variable, the value of the variable can be accessed and modified during the loop. After
the loop the variable takes the value 0. The last iteration of the loop operates with the value 1.
example:
repeat 10 times write “x”; *writes 10 x on the screen*
Code Block:
A code block is a list of code lines between curly braces. A code block can be used in a loop,
interchangeably with a single line of code. By using code blocks, loops may be nested within each
other.
example:
number size;
number sum;
size:= 5;
repeat size times
{ write size and newline;
sum+=size;
}
write newline and “Sum:” and sum;
Output is:
5
4
3
2
1
Sum:15

