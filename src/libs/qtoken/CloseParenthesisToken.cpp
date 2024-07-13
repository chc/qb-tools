#include "CloseParenthesisToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

CloseParenthesisToken::CloseParenthesisToken() {

}
CloseParenthesisToken::~CloseParenthesisToken() {

}
EScriptToken CloseParenthesisToken::GetType() {
    return ESCRIPTTOKEN_CLOSEPARENTH;
}
void CloseParenthesisToken::LoadParams(IStream *stream) {

}
std::string CloseParenthesisToken::ToString() {
    return ")";
}