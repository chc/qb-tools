#include "BreakToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

BreakToken::BreakToken() {

}
BreakToken::~BreakToken() {

}
EScriptToken BreakToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_BREAK;
}
void BreakToken::LoadParams(IStream *stream) {

}
std::string BreakToken::ToString() {
    return "break";
}