# Ela
## Requirements.
to build, you need
- [premake](https://premake.github.io/) to generate project files
alternatively, you could just use the already generate [gnu](https://www.gnu.org)-[make](https://www.gnu.org/software/make/)-[Makefile](https://www.gnu.org/software/make/manual/make.html) which is published in this repo. 
# TODO.
- [x] Type Inference for Variable Definitions
- [x] Array Literals -> check types
- [x] Static Array type: `var x : [int,4];` (shorthand form for `var x : array[int,4];`)
- [x] become more functional, by adding a `Function` type

# Design.
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
    // or how it is imported, because they have not seen the import-statement
    someFuncFromTestModule();
}
import "testModule"
```

## Statements and Expressions.
Expressions work as they normally would in most c-style languages.
unlike java, for example, we *do* allow expression statements

