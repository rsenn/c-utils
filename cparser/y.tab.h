/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
#define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype {
  IDENTIFIER = 258,
  I_CONSTANT = 259,
  F_CONSTANT = 260,
  STRING_LITERAL = 261,
  FUNC_NAME = 262,
  SIZEOF = 263,
  PTR_OP = 264,
  INC_OP = 265,
  DEC_OP = 266,
  LEFT_OP = 267,
  RIGHT_OP = 268,
  LE_OP = 269,
  GE_OP = 270,
  EQ_OP = 271,
  NE_OP = 272,
  AND_OP = 273,
  OR_OP = 274,
  MUL_ASSIGN = 275,
  DIV_ASSIGN = 276,
  MOD_ASSIGN = 277,
  ADD_ASSIGN = 278,
  SUB_ASSIGN = 279,
  LEFT_ASSIGN = 280,
  RIGHT_ASSIGN = 281,
  AND_ASSIGN = 282,
  XOR_ASSIGN = 283,
  OR_ASSIGN = 284,
  TYPEDEF_NAME = 285,
  ENUMERATION_CONSTANT = 286,
  TYPEDEF = 287,
  EXTERN = 288,
  STATIC = 289,
  AUTO = 290,
  REGISTER = 291,
  INLINE = 292,
  CONST = 293,
  RESTRICT = 294,
  VOLATILE = 295,
  BOOL = 296,
  CHAR = 297,
  SHORT = 298,
  INT = 299,
  LONG = 300,
  SIGNED = 301,
  UNSIGNED = 302,
  FLOAT = 303,
  DOUBLE = 304,
  VOID = 305,
  COMPLEX = 306,
  IMAGINARY = 307,
  STRUCT = 308,
  UNION = 309,
  ENUM = 310,
  ELLIPSIS = 311,
  CASE = 312,
  DEFAULT = 313,
  IF = 314,
  ELSE = 315,
  SWITCH = 316,
  WHILE = 317,
  DO = 318,
  FOR = 319,
  GOTO = 320,
  CONTINUE = 321,
  BREAK = 322,
  RETURN = 323,
  ALIGNAS = 324,
  ALIGNOF = 325,
  ATOMIC = 326,
  GENERIC = 327,
  NORETURN = 328,
  STATIC_ASSERT = 329,
  THREAD_LOCAL = 330
};
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define I_CONSTANT 259
#define F_CONSTANT 260
#define STRING_LITERAL 261
#define FUNC_NAME 262
#define SIZEOF 263
#define PTR_OP 264
#define INC_OP 265
#define DEC_OP 266
#define LEFT_OP 267
#define RIGHT_OP 268
#define LE_OP 269
#define GE_OP 270
#define EQ_OP 271
#define NE_OP 272
#define AND_OP 273
#define OR_OP 274
#define MUL_ASSIGN 275
#define DIV_ASSIGN 276
#define MOD_ASSIGN 277
#define ADD_ASSIGN 278
#define SUB_ASSIGN 279
#define LEFT_ASSIGN 280
#define RIGHT_ASSIGN 281
#define AND_ASSIGN 282
#define XOR_ASSIGN 283
#define OR_ASSIGN 284
#define TYPEDEF_NAME 285
#define ENUMERATION_CONSTANT 286
#define TYPEDEF 287
#define EXTERN 288
#define STATIC 289
#define AUTO 290
#define REGISTER 291
#define INLINE 292
#define CONST 293
#define RESTRICT 294
#define VOLATILE 295
#define BOOL 296
#define CHAR 297
#define SHORT 298
#define INT 299
#define LONG 300
#define SIGNED 301
#define UNSIGNED 302
#define FLOAT 303
#define DOUBLE 304
#define VOID 305
#define COMPLEX 306
#define IMAGINARY 307
#define STRUCT 308
#define UNION 309
#define ENUM 310
#define ELLIPSIS 311
#define CASE 312
#define DEFAULT 313
#define IF 314
#define ELSE 315
#define SWITCH 316
#define WHILE 317
#define DO 318
#define FOR 319
#define GOTO 320
#define CONTINUE 321
#define BREAK 322
#define RETURN 323
#define ALIGNAS 324
#define ALIGNOF 325
#define ATOMIC 326
#define GENERIC 327
#define NORETURN 328
#define STATIC_ASSERT 329
#define THREAD_LOCAL 330

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1
#endif

int yyparse(void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
