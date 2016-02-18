/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 1.0                                                         //
// Language:  C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
// Author:      Deepika Lakshmanan                                 //
//              dlakshma@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>

class ConsumeState
{
public:
  ConsumeState();
  virtual ~ConsumeState();
  void attach(std::istream* pIn) { _pIn = pIn; }
  virtual void eatChars() = 0;
  void consumeChars() {
    _pState->eatChars();
    _pState = nextState();
  }
  bool canRead() { return _pIn->good(); }
  std::string getTok() { return token; }
  bool hasTok() { return token.size() > 0; }
  void returnComments(bool doReadComments) { _doReadComments = doReadComments; }
  void setSpecialCharPairs(std::string scp) { _scp = scp; }
  void setSpecialSingleChars(std::string ssc) { _ssc = ssc; }
  ConsumeState* nextState();
  void resetFirst()
  {
    first_ctor = true;
    first_dtor = true;
  }

protected:
  static std::string token;
  static std::istream* _pIn;
  static int prevChar;
  static int currChar;
  static bool inQuotes;
  static int currQuote;
  static bool _doReadComments;
  static bool first_ctor;
  static bool first_dtor;
  static std::string _scp;
  static std::string _ssc;
  static ConsumeState* _pState;
  static ConsumeState* _pEatCppComment;
  static ConsumeState* _pEatCComment;
  static ConsumeState* _pEatWhitespace;
  static ConsumeState* _pEatPunctuator;
  static ConsumeState* _pEatAlphanum;
  static ConsumeState* _pEatQuotedString;
  static ConsumeState* _pEatSpecialCharPair;
  static ConsumeState* _pEatSpecialSingleChars;
  static ConsumeState* _pEatNewline;
};
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
int ConsumeState::prevChar;
int ConsumeState::currChar;
bool ConsumeState::inQuotes = false;
int ConsumeState::currQuote;
bool ConsumeState::_doReadComments = true;
std::string ConsumeState::_scp = "";
std::string ConsumeState::_ssc = "";
bool ConsumeState::first_ctor = true;
bool ConsumeState::first_dtor = true;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatQuotedString = nullptr;
ConsumeState* ConsumeState::_pEatSpecialCharPair = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleChars = nullptr;
ConsumeState* ConsumeState::_pEatNewline;

void testLog(const std::string& msg);

//Returns the next state for the tokenizer based on the current character
ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
  {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
    _pIn->clear();
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }

  if (std::isspace(currChar) && currChar != '\n')
  {
    testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (inQuotes && currChar != currQuote)
  {
    testLog("state: eatQuotedString");
    return _pEatQuotedString;
  }
  if (currChar == '/' && chNext == '/')
  {
    testLog("state: eatCppComment");
    return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*')
  {
    testLog("state: eatCComment");
    return _pEatCComment;
  }
  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }
  if (_scp.find(currChar) != std::string::npos && _scp.find(chNext) != std::string::npos && (currChar == chNext || chNext == '='))
  {
    testLog("state: eatSpecialCharPair");
    return _pEatSpecialCharPair;
  }
  if (_ssc.find(currChar) != std::string::npos && currChar != chNext)
  {
    testLog("state: eatSpecialSingleChar");
    return _pEatSpecialSingleChars;
  }
  if (std::isalnum(currChar))
  {
    testLog("state: eatAlphanum");
    return _pEatAlphanum;
  }
  if (ispunct(currChar))
  {
    testLog("state: eatPunctuator");
    return _pEatPunctuator;
  }
  if (!_pIn->good())
    return _pEatWhitespace;
  throw(std::logic_error("invalid type"));
}

//Handle whitespace
class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

//Return cpp comments as one token
class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
      if (_doReadComments)
        token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};

//Return quoted strings as one token
class EatQuotedString : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n eating quoted string";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      prevChar = currChar;
      currChar = _pIn->get();
      if ((currChar == currQuote) && prevChar == '\\')
      {
        token += currChar;
        if (!_pIn->good())  // end of stream
          return;
        prevChar = currChar;
        currChar = _pIn->get();
      }
    } while (currChar != currQuote);
  }
};

//Return C comment as one token
class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C comment";
    do {
      if (_doReadComments)
        token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
    if (_doReadComments)
    {
      token += currChar;
      token += _pIn->peek();
    }
    _pIn->get();
    currChar = _pIn->get();
  }
};

//Return punctuators in one token
class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating punctuator";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    if (currChar == '\'' || currChar == '"') {
      inQuotes = !inQuotes;
      currQuote = currChar;
      currChar = _pIn->get();
      return;
    }
    currChar = _pIn->get();
    while (ispunct(currChar) && _scp.find(currChar) == std::string::npos && _ssc.find(currChar) == std::string::npos) {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    }
  }
};

//Return alphanumerics as one token
class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar));
  }
};

//Return newline as one token
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating newline";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

//Handle special character pairs
class EatSpecialCharPair : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating special char pair";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

//Handle special single characters
class EatSpecialSingleChars : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating special single chars";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

//ConsumeState constructor
ConsumeState::ConsumeState()
{
  if (first_ctor)
  {
    first_ctor = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatQuotedString = new EatQuotedString();
    _pEatNewline = new EatNewline();
    _pEatSpecialCharPair = new EatSpecialCharPair();
    _pEatSpecialSingleChars = new EatSpecialSingleChars();
    _pState = _pEatWhitespace;
  }
}

//ConsumeState destructor
ConsumeState::~ConsumeState()
{
  if (first_dtor)
  {
    first_dtor = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatQuotedString;
    delete _pEatSpecialCharPair;
    delete _pEatSpecialSingleChars;
    delete _pEatNewline;
  }
}

//Reset 'first' so that multiple toker objects can be created
void Toker::resetFirst()
{
  pConsumer->resetFirst();
}

//Toker constructor
Toker::Toker() : pConsumer(new EatWhitespace()) {}

//Toker destructor
Toker::~Toker() { delete pConsumer; }

//Attach filestream handle to ConsumeState object
bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

//Get token from ConsumeState
std::string Toker::getTok()
{
  while (true)
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

//Return true if filestream is readable
bool Toker::canRead() { return pConsumer->canRead(); }

//Set false to not display comments as tokens (default:true)
void Toker::returnComments(bool doReadComments)
{
  pConsumer->returnComments(doReadComments);
}

//Set special character pairs
void Toker::setSpecialCharPairs(std::string scp)
{
  pConsumer->setSpecialCharPairs(scp);
}

//Set special single characters
void Toker::setSpecialSingleChars(std::string ssc)
{
  pConsumer->setSpecialSingleChars(ssc);
}

//Output log statements
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//------< Test Stub >--------------------------------------------------
#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  std::string fileSpec = "../Tokenizer/tokenizer_test.txt";
  std::ifstream in(fileSpec);

  std::cout << "\n-------------------INPUT-------------------\n\n";
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

  std::cout << "\n\n-------------------OUTPUT-------------------\n";
  while (in.good())
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  }
  in.close();
  getchar();
  return 0;
}
#endif