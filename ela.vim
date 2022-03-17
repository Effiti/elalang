"Copyright 2021 Alexey Kutepov <reximkut@gmail.com> and Porth Contributors
"
"Permission is hereby granted, free of charge, to any person obtaining a copy
""of this software and associated documentation files (the "Software"), to deal
""in the Software without restriction, including without limitation the rights
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
"copies of the Software, and to permit persons to whom the Software is
"furnished to do so, subject to the following conditions:
"
"The above copyright notice and this permission notice shall be included in all
"copies or substantial portions of the Software.
"
"THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
"SOFTWARE.


" Vim syntax file
" Language: Ela

" Usage Instructions
" Put this file in .vim/syntax/ela.vim
" and add in your .vimrc file the next line:
" autocmd BufRead,BufNewFile *.ela set filetype=ela

if exists("b:current_syntax")
  finish
endif

"set iskeyword=a-z,A-Z,-,*,_,!,@,?,\:,\(,\)
syntax keyword elaTodos TODO XXX FIXME NOTE
syntax keyword elaKeywords ? : @ fn int string import var
syntax region elaCommentLine start="//" end="$"   contains=elaTodos
syntax region elaString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=elaEscapes
syntax region elaChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=elaEscapes
syntax match elaNumber display /\d\{-1,}/
syntax keyword elaTypeNames addr int ptr bool

hi def link elaTodos Todo
hi def link elaKeywords Keyword
hi def link elaCommentLine Comment
hi def link elaString String
hi def link elaNumber Number
hi def link elaTypeNames Type
hi def link elaChar Character
hi def link elaEscapes SpecialChar

let b:current_syntax = "ela"
