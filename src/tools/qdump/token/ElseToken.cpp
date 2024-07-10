#include "ElseToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ElseToken::ElseToken() {

}
ElseToken::~ElseToken() {

}
EScriptToken ElseToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ELSE;
}
void ElseToken::LoadParams(IQStream *stream) {

}
std::string ElseToken::ToString() {
    return "else";
}
int ElseToken::GetPreTabOffset() {
    return -1;
}
int ElseToken::GetPostTabOffset() {
    return 1;
}