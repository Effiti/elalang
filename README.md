# Ela
## Requirements
to build, you need
- [premake](https://premake.github.io/) to generate project files
alternatively, you could just use the already generate [gnu](https://www.gnu.org)-[make](https://www.gnu.org/software/make/)-[Makefile](https://www.gnu.org/software/make/manual/make.html) which is published in this repo. 
# Design

## Semicolon
Oh, the age-old question of the semicolon; enemy of any beginner programmer,
friend of some people. 
Why should Ela use a semicolon anywhere? And should semicolons be required everywhere?
I think semicolons are very much needed to avoid parsing ambiguity.

## Import-statements
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
