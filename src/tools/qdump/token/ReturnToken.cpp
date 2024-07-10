#include "ReturnToken.h"
#include "../IQStream.h"

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
void ReturnToken::LoadParams(IQStream *stream) {

}
std::string ReturnToken::ToString() {
    return "return ";
}