#include "ReturnToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ReturnToken::ReturnToken() {

}
ReturnToken::~ReturnToken() {

}
EScriptToken ReturnToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RETURN;
}
void ReturnToken::LoadParams(IStream *stream) {

}
std::string ReturnToken::ToString() {
    return AppendSpaceToString ? "return " : "return";
}