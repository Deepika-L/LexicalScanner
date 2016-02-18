#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
// Author:      Deepika Lakshmanan                                   //
//              dlakshma@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Public Interface
* ------------------
* SemiExp semi;                             //create semi-expression instance
* semi.get();                               //get tokens from tokenizer to create semi-expression
* semi.length();                            //return length of semi-expression
* semi.operator[](n);                       //return token at index n
* semi.find(tok);                           //find tok in semi-expression
* semi.push_back();                         //add token to semi-expression
* semi.remove(tok);                         //remove tok from semi-expression
* semi.remove(i);                           //remove tok from index i
* semi.toLower();                           //convert semi-expression to lowercase
* semi.trimFront();                         //remove leading newlines from semi-expression
* semi.clear();                             //clear the semi-expression
* semi.show();                              //show the semi-expression
* semi.isTerminator(tok);                   //check if tok is a terminator
* semi.isAccessSpecifier();                 //handle semi-expression if an access specifier has been encountered
* semi.handleForLoop();                     //handle for loops while creating semi-expression
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv LexicalScanner.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 02 Feb 2016
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "ITokenCollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    ~SemiExp();
    SemiExp& operator=(const SemiExp&) = delete;
    bool get(bool clear = true);
    size_t length();
    Token& operator[](int n);
    size_t find(const Token& tok);
    void push_back(const Token& tok);
    bool merge(const Token& firstTok, const Token& secondTok);
    bool remove(const Token& tok);
    bool remove(size_t i);
    void toLower();
    void trimFront();
    void clear();
    Token show(bool showNewLines = false);
    bool isTerminator(Token tok);
    bool isAccessSpecifier();
    bool handleForLoop();
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
  };
}
#endif