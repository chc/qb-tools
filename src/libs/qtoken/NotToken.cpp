#include "NotToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

NotToken::NotToken() {

}
NotToken::~NotToken() {

}
EScriptToken NotToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_NOT;
}
void NotToken::LoadParams(IStream *stream) {

}
std::string NotToken::ToString() {
    return AppendSpaceToString ? "NOT " : "NOT";
}