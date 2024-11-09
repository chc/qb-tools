#include "NotEqualToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

NotEqualToken::NotEqualToken() {

}
NotEqualToken::~NotEqualToken() {

}
EScriptToken NotEqualToken::GetType() {
    return ESCRIPTTOKEN_NOTEQUAL;
}
void NotEqualToken::LoadParams(IStream *stream) {

}
std::string NotEqualToken::ToString() {
    return AppendSpaceToString ? "!= " : "!=";
}