# Design

## Return values
### Error type
Most libraries use an `int` to indicate errors. Yet, they interpret them all
differently:
* is a positive number an error?
* does `0` mean success?
* is it always -1 or can there be lower values?(-ENOMEM vs -1 and errno)
* which error numbers are there?(not ever library uses `errno.h`)

This causes certain issues when handling multiple error types at once:
* comparing the `int` against the wrong error values
  (`ESP_ERR_NO_MEM` vs `ENOMEM`)
* treating an error code as an actual integer (`snprintf`)

In addition to that, it is common to mix error codes and data in the same
return value by using 0 and negative numbers for errors and positive numbers
for data. This might seem easier and more effecient but causes several more
issues:
* reducing the number of bits for the positive part. `write` takes a `size_t`
  but passing values greater than `SSIZE_MAX` is implementation-defined due to
  the return value being `ssize_t`.
* Forgetting to handle an error results in using a negative number to code that
  was not written to handle that. Either signed numbers are being use
  throughout the rest of the code possibly causing buffer underflows or the
  negative number is being cast to a positive one possibly causing overflows
  due to wrap-around.

To prevent all of this, `struct result` is used which has a single field
`int reason` and is only used for the error reason, never for data. See it's
documentation for details.

### Return structs as values
Inlined Functions which can never fail and need to return data, return the
structs directly instead of requiring a pointer-argument. That greatly
simplifies the usage due to reduced boilerplate.

The reason why this is limited to inlined functions is that only then the stack
usage will be unaffected in case you want to move the data to the heap
immediately after the return. Usually, this only works with optimizations
enabled.

### Ease of use
For all APIs, the `struct result` is the single source of truth to verify if
the call was successful or not. There are alternatives to functions like
`snprintf` which are infamously complicated to use correctly.

## No global state
This library does not read or write any global state, not even `errno`.
If global state of external libraries is modified, the function documentation
clearly states that.

## Integer conversion
This library ALWAYS checks for overflow, underflow and truncation before
attempting integer conversion. `-ERANGE` is returned in case it would fail.

The functions for simplifying those checks are available in the public
interface so users can do the same in their code.

## Variable safety

### Summary
* This library does **not** use `-Wdeclaration-after-statement`
* This library **does** use `-Wshadow
* This library uses `const` as much as possible

See the reasoning for these decisions is the sections below.

### Constness
Since C variables are mutable by default, all variables that do not require
modification shall be declared `const`. Restructuring code to make more
variables const is desirable but should not come at the cost of readability or
simplicity.

### How C works
1) You can redeclare a variable with the same type
2) You can **NOT** redeclare a variable with a different type within the same block
3) You can redeclare a variable with a different type in a child-block
4) You can declare any type and initialize it later - even non-pointer structs
5) You can declare a variable in `for`
   a) the loop header is a child block of the current block
   b) the loop body is a child block of the loop header

### `-Wdeclaration-after-statement`
Originally this was a compiler-limitation but eventually it turned into a
style-decision. The most popular argument is that it makes the code more
readable and prevents bugs due to misunderstood scoping rules.

I failed to find any good examples of forward-declaring variables at the top of
a function aiding safety rather than style. So here's the SAFETY reasons for
**not** using this style:
* forward-declaring variables increases the number of lines which can use the
  variable
* preventing use before initialization relies on `-Wuninitialized` which seems
  to be heuristic
* it prevents making variables `const`

### `-Wshadow`
This prevents you from reading or writing the wrong variable which just
happened to be in scope. This issue is infamously popular in nested for-loops.

This warning does come with the annoyance of using unique variable names.
Different environments may have different global variables which means
perfectly valid code can fail to compile after including a new header or
compiling with a different libc. This downside is accepted in favor of safety.

This warning makes redeclaring variables in the current scope more consistent
by disabling it completely. Without `-Wshadow`, you'll only get an error if the
new type is different.
