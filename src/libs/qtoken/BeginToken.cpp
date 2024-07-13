#include "BeginToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

BeginToken::BeginToken() {

}
BeginToken::~BeginToken() {

}
EScriptToken BeginToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_BEGIN;
}
void BeginToken::LoadParams(IStream *stream) {

}
std::string BeginToken::ToString() {
    return "begin";
}