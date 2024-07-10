#include "AllArgsToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

AllArgsToken::AllArgsToken() {

}
AllArgsToken::~AllArgsToken() {

}
EScriptToken AllArgsToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ALLARGS;
}
void AllArgsToken::LoadParams(IQStream *stream) {

}
std::string AllArgsToken::ToString() {
    return "<...>";
}