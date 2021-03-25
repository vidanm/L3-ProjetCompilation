/* A Bison parser, made by GNU Bison 3.7.3.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_AS_TAB_H_INCLUDED
# define YY_YY_SRC_AS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TYPE = 258,                    /* TYPE  */
    CHARACTER = 259,               /* CHARACTER  */
    NUM = 260,                     /* NUM  */
    IDENT = 261,                   /* IDENT  */
    ORDER = 262,                   /* ORDER  */
    DIVSTAR = 263,                 /* DIVSTAR  */
    ADDSUB = 264,                  /* ADDSUB  */
    EQ = 265,                      /* EQ  */
    AND = 266,                     /* AND  */
    OR = 267,                      /* OR  */
    RETURN = 268,                  /* RETURN  */
    WHILE = 269,                   /* WHILE  */
    ELSE = 270,                    /* ELSE  */
    IF = 271,                      /* IF  */
    PRINT = 272,                   /* PRINT  */
    VOID = 273,                    /* VOID  */
    READE = 274,                   /* READE  */
    READC = 275,                   /* READC  */
    STRUCT = 276                   /* STRUCT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TYPE 258
#define CHARACTER 259
#define NUM 260
#define IDENT 261
#define ORDER 262
#define DIVSTAR 263
#define ADDSUB 264
#define EQ 265
#define AND 266
#define OR 267
#define RETURN 268
#define WHILE 269
#define ELSE 270
#define IF 271
#define PRINT 272
#define VOID 273
#define READE 274
#define READC 275
#define STRUCT 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union value
{

	Node *node;
	char character;
	int integer;
	char identifier[64];


};
typedef union value YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_AS_TAB_H_INCLUDED  */
