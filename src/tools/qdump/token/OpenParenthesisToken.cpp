#include "OpenParenthesisToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

OpenParenthesisToken::OpenParenthesisToken() {

}
OpenParenthesisToken::~OpenParenthesisToken() {

}
EScriptToken OpenParenthesisToken::GetType() {
    return ESCRIPTTOKEN_OPENPARENTH;
}
void OpenParenthesisToken::LoadParams(IQStream *stream) {

}
std::string OpenParenthesisToken::ToString() {
    return "(";
}