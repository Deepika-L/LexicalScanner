///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
// Author:      Deepika Lakshmanan                                   //
//              dlakshma@syr.edu                                     //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExpression.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

//Constructor
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

//Destructor
SemiExp::~SemiExp() {}

//Return size of semi-expression vector
size_t SemiExp::length() {
  return _tokens.size();
}

//Check if an access specifier has been encountered
bool SemiExp::isAccessSpecifier()
{
  size_t n = SemiExp::find("public");
  if (n < length())
    return true;

  n = SemiExp::find("private");
  if (n < length())
    return true;

  n = SemiExp::find("protected");
  if (n < length())
    return true;
  return false;
}

//Handle semicolons within a for loop
bool SemiExp::handleForLoop()
{
  int nSemicolon = 2;
  while (true)
  {
    Token token = _pToker->getTok();
    _tokens.push_back(token);

    if (token == ":")
    {
      nSemicolon = 0;
    }
    if (token == ";")
    {
      if (nSemicolon == 0)
      {
        return true;
      }
      if (nSemicolon > 0)
      {
        nSemicolon--;
      }
    }
    if (token == "{" || token == "}")
      return true;
  }
}

//Check if token is a terminator
bool SemiExp::isTerminator(Token token)
{
  if (token == "for")
    return handleForLoop();

  if (token == "{" || token == "}" || token == ";")
    return true;

  if (token == "\n")
  {
    size_t n = find("#");
    if (n < length())
      return true;
  }

  if (token == ":" && isAccessSpecifier())
    return true;

  return false;
}

//Get tokens and create a semi-expression
bool SemiExp::get(bool clear)
{
  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));

  if (clear)
    _tokens.clear();

  std::string token;
  do
  {
    token = _pToker->getTok();

    if (token == "")
      return false;

    _tokens.push_back(token);

  } while (!isTerminator(token));
  trimFront();
  return true;
}

//Return token at specified index
Token& SemiExp::operator[](int n)
{
  if (n >= (int)_tokens.size() || n < 0)
    throw(std::invalid_argument("Index not in range"));
  return _tokens[n];
}

//Check if the specified token is in the semi-expression
size_t SemiExp::find(const Token& tok)
{
  for (size_t i = 0; i < length(); ++i)
  {
    if (tok == _tokens[i])
      return i;
  }
  return length();
}

//Add token to the semi-expression
void SemiExp::push_back(const Token& tok)
{
  _tokens.push_back(tok);
}

//Merge tokens
bool SemiExp::merge(const Token& firstTok, const Token& secondTok)
{
  return true;
}

//Remove token if found in semi-expression
bool SemiExp::remove(const Token& tok)
{
  for (size_t i = 0; i < length(); ++i)
  {
    if (tok == _tokens[i])
    {
      _tokens.erase(_tokens.begin() + i);
      return true;
    }
  }
  return false;
}

//Remove token at specified index
bool SemiExp::remove(size_t i)
{
  if (_tokens.size() <= i || i < 0)
    return false;
  _tokens.erase(_tokens.begin() + i);
  return true;
}

//Make tokens lowercase
void SemiExp::toLower()
{
  for (size_t i = 0; i < length(); ++i)
  {
    for (size_t j = 0; j < _tokens[i].length(); ++j)
    {
      _tokens[i][j] = tolower(_tokens[i][j]);
    }
  }
}

//Remove leading newlines
void SemiExp::trimFront()
{
  for (int i = 0; i < (int)length(); i++)
  {
    if (_tokens[0] == "\n" || _tokens[0] == "")
    {
      remove(0);
      i = 0;
    }
    else
    {
      break;
    }
  }
}

//Empty the semi-expression vector
void SemiExp::clear()
{
  _tokens.clear();
}

//Return the semi-expression
Token SemiExp::show(bool showNewLines)
{
  std::cout << "\n";
  if (_tokens.size() == 0)
    return "";
  Token temp;
  for (auto token : _tokens)
    if (token != "\n" || showNewLines)
      temp.append(" ").append(token);
  return temp;
}

//----< Test Stub >--------------------------------------------------
#ifdef TEST_SEMIEXPRESSION
int main()
{
  std::string fileSpec = "../SemiExpression/semiexpression_test.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }

  std::cout << "\n-------------------INPUT-------------------\n\n";
  std::string line;
  if (in.is_open())
  {
    while (getline(in, line))
    {
      std::cout << line << '\n';
    }
  }
  in.clear();
  in.seekg(0, std::ios::beg);

  Toker toker;
  toker.attach(&in);
  //Setting characters to be considered as special tokens
  toker.setSpecialCharPairs("<<>>++--==+=-=*=/=::");
  toker.setSpecialSingleChars("<>[](){}:=+-*");
  //Disabling comments from being returned from tokenizer
  toker.returnComments(0);

  std::cout << "\n\n-------------------OUTPUT-------------------\n";
  SemiExp semi(&toker);
  while (semi.get())
  {
    std::cout << "\n -- semiExpression --";
    std::cout << semi.show(1) << "\n";
  }
  /*
  May have collected tokens, but reached end of stream
  before finding SemiExp terminator.
  */
  if (semi.length() > 0)
  {
    std::cout << "\n -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  getchar();
  return 0;
}
#endif