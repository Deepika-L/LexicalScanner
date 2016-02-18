///////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Automated test suite for the Lexical Scanner  //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Test Executive                                       //
// Author:      Deepika Lakshmanan                                   //
//              dlakshma@syr.edu                                     //
///////////////////////////////////////////////////////////////////////

#include "../Tokenizer/Tokenizer.h"
#include "../SemiExpression/SemiExpression.h"
#include "../TestExecutive/TestExecutive.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

using namespace Scanner;

//Test for collection of alphanumeric and punctuator tokens
bool TestExecutive::Alphanum_punct()
{
  std::string fileSpec = "../TestExecutive/tokenizer_tests/alphanum_punct.txt";
  std::ifstream in(fileSpec);
  std::cout << "\nREQUIREMENT 4.1, 4.2\n";
  std::cout << "Demonstrating collection of alphanumeric and punctuator tokens\n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
  std::string line;
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  if (in.is_open())
  {
    while (getline(in, line))
    {
      std::cout << line << '\n';
    }
  }
  in.clear();
  in.seekg(0, std::ios::beg);

  //Setting characters to be considered as special tokens
  Toker toker;
  toker.attach(&in);
  toker.setSpecialCharPairs("<<>>++--==+=-=*=/=::");
  toker.setSpecialSingleChars("<>[](){}:=+-*");

  std::cout << "\n\nTOKENIZER OUTPUT:\n";
  while (in.good())
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  }
  toker.resetFirst();
  in.close();
  return 0;
}

//Test for collection of special single characters and special character pairs
bool TestExecutive::Special_chars()
{
  std::string fileSpec = "../TestExecutive/tokenizer_tests/special_chars.txt";
  std::ifstream in(fileSpec);
  std::cout << "\nREQUIREMENT 4.3\n";
  std::cout << "Demonstrating collection of special single characters and special character pairs\n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
  std::string line;
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  if (in.is_open())
  {
    while (getline(in, line))
    {
      std::cout << line << '\n';
    }
  }
  in.clear();
  in.seekg(0, std::ios::beg);

  //Setting characters to be considered as special tokens
  Toker toker;
  toker.attach(&in);
  toker.setSpecialCharPairs("<<>>++--==+=-=*=/=::");
  toker.setSpecialSingleChars("<>[](){}:=+-*");

  std::cout << "\n\nTOKENIZER OUTPUT:\n";
  while (in.good())
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  }
  in.close();
  toker.resetFirst();
  return 0;
}

//Test for collection of C comments, Cpp comments and quoted strings
bool TestExecutive::Comments_strings()
{
  std::string fileSpec = "../TestExecutive/tokenizer_tests/comments_strings.txt";
  std::ifstream in(fileSpec);
  std::cout << "\nREQUIREMENT 4.4, 4.5, 4.6\n";
  std::cout << "Demonstrating collection of C comments, Cpp comments and quoted strings\n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
  std::string line;
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  if (in.is_open())
  {
    while (getline(in, line))
    {
      std::cout << line << '\n';
    }
  }
  in.clear();
  in.seekg(0, std::ios::beg);

  //Setting characters to be considered as special tokens
  Toker toker;
  toker.attach(&in);
  toker.setSpecialCharPairs("<<>>++--==+=-=*=/=::");
  toker.setSpecialSingleChars("<>[](){}:=+-*");

  std::cout << "\n\nTOKENIZER OUTPUT:\n";
  while (in.good())
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  }
  in.close();
  toker.resetFirst();
  return 0;
}

//Test for termination of semi-expression after extracting '}', '{' and ';'
bool TestExecutive::SemiExp_terminators()
{
  std::string fileSpec = "../TestExecutive/semiexpression_tests/semiexp_terminators.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  std::cout << "\nREQUIREMENT 7\n";
  std::cout << "Demonstrating termination of semi-expression after extracting '}', '{' and ';'\n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
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

  std::cout << "\n\nSEMIEXPRESSION OUTPUT:\n";
  SemiExp semi(&toker);
  while (semi.get())
  {
    std::cout << "\n -- semiExpression --";
    std::cout << semi.show(1) << "\n";
  }
  /* May have collected tokens, but reached end of stream
  before finding SemiExp terminator.*/
  if (semi.length() > 0)
  {
    std::cout << "\n -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  toker.resetFirst();
  return 0;
}

//Test for termination of semi-expression after ':' when preceeded by access specifier
bool TestExecutive::SemiExp_accspecifier()
{
  std::string fileSpec = "../TestExecutive/semiexpression_tests/semiexp_accspecifier.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  std::cout << "\nREQUIREMENT 7\n";
  std::cout << "Demonstrating termination of semi-expression after ':' when preceeded by access specifier \n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
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

  std::cout << "\n\nSEMIEXPRESSION OUTPUT:\n";
  SemiExp semi(&toker);
  while (semi.get())
  {
    std::cout << "\n -- semiExpression --";
    std::cout << semi.show(1) << "\n";
  }
  /* May have collected tokens, but reached end of stream
  before finding SemiExp terminator.*/
  if (semi.length() > 0)
  {
    std::cout << "\n -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  toker.resetFirst();
  return 0;
}

//Test for handling of ';' inside for loops in semi-expression
bool TestExecutive::SemiExp_forloop()
{
  std::string fileSpec = "../TestExecutive/semiexpression_tests/semiexp_forloop.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  std::cout << "\nREQUIREMENT 8\n";
  std::cout << "Demonstrating handling of ';' inside for loops in semi-expression\n";
  std::cout << "CONTENTS OF FILE INPUT:\n\n";
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

  std::cout << "\n\nSEMIEXPRESSION OUTPUT:\n";
  SemiExp semi(&toker);
  while (semi.get())
  {
    std::cout << "\n -- semiExpression --";
    std::cout << semi.show(1) << "\n";
  }
  /* May have collected tokens, but reached end of stream
  before finding SemiExp terminator.*/
  if (semi.length() > 0)
  {
    std::cout << "\n -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  toker.resetFirst();
  return 0;
}

//Test to demonstrate implementation of ITokCollection functions
bool TestExecutive::itokcollection_test()
{
  std::cout << "\nREQUIREMENT 9\n";
  std::cout << "Demonstrating implementation of all functions in ITokCollection\n";
  std::cout << "Refer to lines 22-213 in SemiExpression.cpp to see implementation";
  SemiExp itok_test;
  itok_test.push_back("\n");
  itok_test.push_back("\n");
  itok_test.push_back("Token1");
  itok_test.push_back("Token2");
  itok_test.push_back("Token3");

  std::cout << "\n\n Testing SemiExp::show()" << "\n -----------------------------";
  std::cout << " The semi-expression:\n " << itok_test.show(1);

  std::cout << "\n\n Testing SemiExp::trimFront()" << "\n -----------------------------";
  itok_test.trimFront();
  std::cout << " The semi-expression: " << itok_test.show(1);

  std::cout << "\n\n Testing SemiExp::push_back()" << "\n -----------------------------";
  itok_test.push_back("Token4");
  std::cout << " Adding Token4 to semi-expression: " << itok_test.show(1);

  std::cout << "\n\n Testing SemiExp::length()" << "\n -----------------------------";
  std::cout << "\n Length of the semi-expression: " << itok_test.length();

  std::cout << "\n\n Testing SemiExp::operator[]" << "\n -----------------------------";
  std::cout << "\n The token at index 2 of semi-expression: " << itok_test.operator[](2);

  std::cout << "\n\n Testing SemiExp::find()" << "\n -----------------------------";
  std::cout << "\n Find Token2 in semiexpression at index: " << itok_test.find("Token2");

  std::cout << "\n\n Testing SemiExp::remove()" << "\n -----------------------------";
  itok_test.remove(2);
  std::cout << " Remove token in semiexpression at index 2: " << itok_test.show(0);

  std::cout << "\n\n Testing SemiExp::remove()" << "\n -----------------------------";
  itok_test.remove("Token4");
  std::cout << " Remove Token4 from semiexpression: " << itok_test.show(0);

  std::cout << "\n\n Testing SemiExp::toLower()" << "\n -----------------------------";
  itok_test.toLower();
  std::cout << " Convert semi-expression to lowercase: " << itok_test.show(0);

  std::cout << "\n\n Testing SemiExp::clear()" << "\n -----------------------------";
  itok_test.clear();
  std::cout << " Contents of semi-expression after calling clear(): " << itok_test.show(0) << "\n\n";

  return 0;
}


//----< Test Stub >--------------------------------------------------
#ifdef TEST_EXECUTIVE
int main()
{
  TestExecutive test;
  //Tokenizer requirement 4.1, 4.2
  if (test.Alphanum_punct())
    return 1;

  std::cout << "\n\n -----------------------------------------------------------------------------------------\n";

  //Tokenizer requirement 4.3
  if (test.Special_chars())
    return 1;

  std::cout << "\n\n -----------------------------------------------------------------------------------------\n";

  //Tokenizer requirement 4.4, 4.5, 4.6
  if (test.Comments_strings())
    return 1;

  std::cout << "\n\n -----------------------------------------------------------------------------------------\n";

  //Semi-expression requirement 7
  if (test.SemiExp_terminators())
    return 1;

  std::cout << "\n -----------------------------------------------------------------------------------------\n";

  //Semi-expression requirement 7
  if (test.SemiExp_accspecifier())
    return 1;

  std::cout << "\n -----------------------------------------------------------------------------------------\n";

  //Semi-expression requirement 8
  if (test.SemiExp_forloop())
    return 1;

  std::cout << "\n -----------------------------------------------------------------------------------------\n";

  //Semi-expression requirement 8
  if (test.itokcollection_test())
    return 1;

  getchar();
  return 0;
}
#endif
