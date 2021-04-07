/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_SRC_AS_TAB_H_INCLUDED
# define YY_YY_SRC_AS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHARACTER = 258,
    NUM = 259,
    IDENT = 260,
    SIMPLETYPE = 261,
    ORDER = 262,
    EQ = 263,
    ADDSUB = 264,
    DIVSTAR = 265,
    OR = 266,
    AND = 267,
    STRUCT = 268,
    IF = 269,
    WHILE = 270,
    RETURN = 271,
    VOID = 272,
    PRINT = 273,
    READC = 274,
    READE = 275,
    ELSE = 276
  };
#endif
/* Tokens.  */
#define CHARACTER 258
#define NUM 259
#define IDENT 260
#define SIMPLETYPE 261
#define ORDER 262
#define EQ 263
#define ADDSUB 264
#define DIVSTAR 265
#define OR 266
#define AND 267
#define STRUCT 268
#define IF 269
#define WHILE 270
#define RETURN 271
#define VOID 272
#define PRINT 273
#define READC 274
#define READE 275
#define ELSE 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{


    int integer;
    char character;
    char identifier[64];
    Node *node;


};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_AS_TAB_H_INCLUDED  */
