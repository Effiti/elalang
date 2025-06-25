";" @punctuation
[
  "if"
  "else"
  "import"
  "fn"
  "return"
  "for"
  "var" 
  "extern"
] @keyword
"," @punctuation.delimiter
[
  "["
  "]"
  "{"
  "}"
] @punctuation.bracket
[
  "+"
  "-"
  "*"
  "/"
  "=="
  "!="
  "<"
  "<="
  ">"
  ">="
  "&&"
  "||"
  "!"
  "++"
  "--"
  "&"
] @operator
(type) @type
(number) @number
(string) @string
(character) @string
(functionHeader
  name: (ident) @function
)
(comment) @comment
(call
  function: (ident) @function
)
(assignment
  name: (ident) @variable
)
(vardef
  name: (ident) @variable
)
