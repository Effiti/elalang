# Ela
## Requirements.
to build, you need
- [premake](https://premake.github.io/) to generate project files
alternatively, you could just use the already generate [gnu](https://www.gnu.org)-[make](https://www.gnu.org/software/make/)-[Makefile](https://www.gnu.org/software/make/manual/make.html) which is published in this repo. 
# Design.
## TODO.
- [ ] Type Inference for Variable Definitions
- [x] Static Array type: `var x : [int,4];` (shorthand form for `var x : array[int,4];`)
- [ ] become more functional, by adding a `function[tuple[x, x1, x2, x3], y]`-type with a possible shorthand form of `func()->array[func(int, float)->[int, 12], 12]` or `(int,int)->(int,int)->(int,int)->int` (a first step towards lambda functions)

## Import-statements.
import-statements currently have to appear in the beginning of the file.
This makes the program easier to read, and also avoids
any debate about when to put imported symbols into scope, i.e.:
```text
func main( args : list[string]) -> int {
    print("hallo welt");

}
func testFunc() {
    // is this function in scope?
    // even if it is, the reader may not know where it comes from,
    // or how it was imported, because he has not seen the import-statement
    someFuncFromTestModule();
}
import "testModule"
```

## Statements and Expressions.
Expressions work as they normally would in most c-style languages.
unlike java, for example, we *do* allow expression statements

