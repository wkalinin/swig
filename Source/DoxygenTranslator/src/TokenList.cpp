/* ----------------------------------------------------------------------------- 
 * This file is part of SWIG, which is licensed as a whole under version 3 
 * (or any later version) of the GNU General Public License. Some additional
 * terms also apply to certain portions of SWIG. The full details of the SWIG
 * license and copyrights can be found in the LICENSE and COPYRIGHT files
 * included with the SWIG source code as distributed by the SWIG developers
 * and at http://www.swig.org/legal.html.
 *
 * TokenList.cpp
 * ----------------------------------------------------------------------------- */

#include "TokenList.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include "swig.h"
#include "Token.h"
#include "DoxygenEntity.h"
#define TOKENSPERLINE 8;	//change this to change the printing behaviour of the token list

using namespace std;

TokenList TokenList::tokenizeDoxygenComment(const std::string &doxygenComment, const std::string &fileName, int fileLine) {
  TokenList tokList;
  tokList.fileLine = fileLine;
  tokList.fileName = fileName;

  bool isPlainString = false;
  string::size_type pos, lastPos = 0;
  char prevChar = doxygenComment[lastPos];
  string currentWord;
  while (true) {
    isPlainString = false;
    pos = doxygenComment.find_first_of("\\@\t\n ", lastPos);
    if (pos == string::npos)
      pos = doxygenComment.size();

    currentWord = doxygenComment.substr(lastPos, pos-lastPos);
    if (prevChar == '\n')
      tokList.m_tokenList.push_back(Token(END_LINE, "\n"));
    else if (prevChar == '\\' || prevChar == '@') {
      // it's a doxygen command
      // hack to get commands like \\ or \@ or @\ or @@
      if (doxygenComment[pos] == '@' || doxygenComment[pos] == '\\') {
        currentWord += doxygenComment[pos];
        pos++;
      }
      tokList.m_tokenList.push_back(Token(COMMAND, currentWord));
    }
    else if (currentWord.size() && (currentWord[0] == '!' || currentWord[0] == '*' || currentWord[0] == '/')) {
      // check if it's one of the '!!!', '***', '///' of any length
      char c = currentWord[0];
      isPlainString = false;
      for (size_t i=0; i<currentWord.size(); i++)
        if (currentWord[i] != c) {
          isPlainString = true;
          break;
        }
    }
    else
      isPlainString = true;

    if (isPlainString && currentWord.size())
      tokList.m_tokenList.push_back(Token(PLAINSTRING, currentWord));

    prevChar = doxygenComment[pos];
    lastPos = pos + 1;
    if (lastPos >= doxygenComment.size())
      break;
  }
  tokList.m_tokenListIter = tokList.m_tokenList.begin();
  return tokList;
}

TokenList::TokenList()
: fileName(""), fileLine(0) {
  m_tokenListIter = m_tokenList.begin();
}


TokenList::~TokenList() {
}

Token TokenList::peek() {
  if (m_tokenListIter != m_tokenList.end()) {
    Token returnedToken = (*m_tokenListIter);
    return returnedToken;
  } else
    return Token(0, "");
}


Token TokenList::next() {
  if (m_tokenListIter != m_tokenList.end()) {
    Token returnedToken = (*m_tokenListIter);
    m_tokenListIter++;
    return (returnedToken);
  } else
    return Token(0, "");
}


list < Token >::iterator TokenList::end() {
  return m_tokenList.end();
}


list < Token >::iterator TokenList::current() {
  return m_tokenListIter;
}


list < Token >::iterator TokenList::iteratorCopy() {
  return m_tokenListIter;
}


void TokenList::setIterator(list < Token >::iterator newPosition) {
  m_tokenListIter = newPosition;
}


void TokenList::printList() {
  list < Token >::iterator p = m_tokenList.begin();
  int i = 1;
  int b = 0;
  while (p != m_tokenList.end()) {
    cout << (*p).toString() << " ";
    b = i % TOKENSPERLINE;
    if (b == 0)
      cout << endl;
    p++;
    i++;
  }
}

void TokenList::printListError(int warningType, std::string message) {
  int curLine = fileLine;
  for (list< Token >::iterator it = m_tokenList.begin(); it != current(); it++)
    if (it->tokenType == END_LINE)
      curLine++;
  Swig_warning(warningType, fileName.c_str(), curLine, "Doxygen parser warning: %s. \n", message.c_str());
}
