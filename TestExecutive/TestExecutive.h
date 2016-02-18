#ifndef TESTEXECUTIVE_H
#define TESTEXECUTIVE_H
///////////////////////////////////////////////////////////////////////
// TestExecutive.h - Automated test suite for the Lexical Scanner    //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Test Executive                                       //
// Author:      Deepika Lakshmanan                                   //
//              dlakshma@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides an automated test suite that demonstrates the
* requirements for Project #1 of CSE687 - OOD, Spring 2016.
*
* Public Interface
* ------------------
* TestExecutive test;                        //create test executive instance
* test.Alphanum_punct();                     //test for collection of alphanumeric and punctuator tokens
* test.Special_chars();                      //test for collection of special single characters and special character pairs
* test.Comments_strings();                   //test for collection of C comments, Cpp comments and quoted strings
* test.SemiExp_terminators();                //test for termination of semi-expression after extracting '}', '{' and ';'
* test.SemiExp_accspecifier();               //test for termination of semi-expression after ':' when preceeded by access specifier
* test.SemiExp_forloop();                    //test for handling of ';' inside for loops in semi-expression
* test.itokcollection_test();                //test to demonstrate implementation of ITokCollection functions
*
* Build Process:
* --------------
* Required Files: SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp, TestExecutive.h, TestExecutive.cpp
* Build Command: devenv LexicalScanner.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2016
*/

class TestExecutive
{
public:
  bool Alphanum_punct();
  bool Special_chars();
  bool Comments_strings();
  bool SemiExp_terminators();
  bool SemiExp_accspecifier();
  bool SemiExp_forloop();
  bool itokcollection_test();
};
#endif