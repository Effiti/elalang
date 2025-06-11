/**
 * @file A tree-sitter grammar for the Ela language
 * @author Effiti
 * @license Apache 2.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "ela",
  extras: $ => [$.comment, /[\s\p{Zs}\uFEFF\u2028\u2029\u2060\u200B]/],

  rules: {
    // TODO: add the actual grammar rules
    source_file: $ => repeat($._def),
    _def: $ => choice($.function,
      $.externFn),
    function: $ => seq($.functionHeader, $.block),
    functionHeader: $ => seq('fn', field('name', $.ident), '(', optional(field('params', $._args)), ')', '->', field('type', $.type)),
    externFn: $ => seq('extern', $.functionHeader, ';'),
    ident: _$ => /[A-z][A-z0-9_]*/,
    type: $ => seq(optional('&'), $.ident, optional(seq('[', $.type, ']'))),
    _args: $ => seq($._param, repeat(seq(',', $._param))),
    _param: $ => seq($.ident, ':', $.type),
    block: $ => seq('{', repeat($.statement), '}'),
    statement: $ => choice($.vardef, $._ifStmt, $._whileStmt, $._forStmt, $._returnStmt, $._exprStmt, $.block),
    vardef: $ => seq('var', field('name', $.ident), ':', optional(field('type', $.type)), optional(seq('=', $.expression)), ';'),
    _ifStmt: $ => seq('if', '(', $.expression, ')', $.statement),
    _whileStmt: $ => seq('while', '(', $.expression, ')', $.statement), 
    _forStmt: $ => seq('for', '(', $.statement, $.statement, $.statement, ')', ),
    _returnStmt: $ => seq('return', $.expression, ';'),
    _exprStmt: $ => seq($.expression, ';'),
    expression: $ => choice(
      $.assignment,
      $.binary_expr,
      $.unary_expr,
      $.call,
      $._primary,
      $._parenthesized
    ),

    assignment: $ => seq(
      field('name', $.ident),
      '=',
      $.expression
    ),

    binary_expr: $ => choice(
      ...[
        ['+', 'left'],
        ['-', 'left'],
        ['*', 'left'],
        ['/', 'left'],
        ['==', 'left'],
        ['!=', 'left'],
        ['<', 'left'],
        ['<=', 'left'],
        ['>', 'left'],
        ['>=', 'left'],
        ['&&', 'left'],
        ['||', 'left'],
      ].map(([operator, assoc]) =>
        assoc === 'left' ?
          prec.left(1, seq(field('left', $.expression), operator, field('right', $.expression))) :
          prec.right(1, seq(field('left', $.expression), operator, field('right', $.expression)))
      )
    ),

    unary_expr: $ => choice(
      seq('!', field('expr', $.expression)),
      seq('-', field('expr', $.expression)),
      seq('++', field('expr', $.ident)),
      seq('--', field('expr', $.ident)),
      seq(field('expr', $.ident), '++'),
      seq(field('expr', $.ident), '--'),
      seq('+', field('expr', $.ident)),
      seq('&', field('expr', $.ident))
    ),

    call: $ => seq(
      field('function', $.ident),
      '(',
      optional(field('args', $.argument_list)),
      ')'
    ),

    argument_list: $ => seq(
      $.expression,
      repeat(seq(',', $.expression))
    ),

    _primary: $ => choice(
      $.number,
      $.string,
      $.ident,
      'true',
      'false',
      'null'
    ),

    _parenthesized: $ => seq(
      '(',
      $.expression,
      ')'
    ),

    number: _$ => /\d+(\.\d+)?/,

    string: _$ => /"(?:[^"\\]|\\.)*"/,

    comment: _$ => token(choice(
      seq('//', /.*/),
      seq('/*', /[^*]*\*+([^/*][^*]*\*+)*/, '/')
    )),
   }
});
