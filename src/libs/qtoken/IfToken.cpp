#include "IfToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

IfToken::IfToken() {

}
IfToken::~IfToken() {

}
EScriptToken IfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_IF;
}
void IfToken::LoadParams(IStream *stream) {

}
std::string IfToken::ToString() {
    return "if ";
}