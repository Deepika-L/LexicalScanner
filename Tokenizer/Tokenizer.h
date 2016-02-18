#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 1.0                                                           //
// Language:  C++, Visual Studio 2015                                //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
// Author:      Deepika Lakshmanan                                   //
//              dlakshma@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Public Interface
 *------------------
 * Toker t;                             //create tokenizer instance
 * t.attach(&in);                       //attach file handler to tokenizer
 * t.getTok();                          //get single token from filestream
 * t.canRead();                         //true if file is readable
 * t.returnComments(0);                 //prevents comments from being returned by tokenizer when set to false
 * t.setSpecialCharPairs(scp);          //set special character pairs by passing scp
 * t.setSpecialSingleChars(ssc);        //set special single character tokens by passing ssc
 * t.resetFirst();                      //reset constructor and destructor so that toker can be reinitialized
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv LexicalScanner.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 2 Feb 2016
 */
#include <iosfwd>
#include <string>

class ConsumeState;

class Toker
{
public:
  Toker();
  ~Toker();
  bool attach(std::istream* pIn);
  std::string getTok();
  bool canRead();
  void returnComments(bool doReadComments);
  void setSpecialCharPairs(std::string scp);
  void setSpecialSingleChars(std::string ssc);
  void resetFirst();
private:
  ConsumeState* pConsumer;
};
#endif
