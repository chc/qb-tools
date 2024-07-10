#include "RepeatToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RepeatToken::RepeatToken() {

}
RepeatToken::~RepeatToken() {

}
EScriptToken RepeatToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_REPEAT;
}
void RepeatToken::LoadParams(IQStream *stream) {

}
std::string RepeatToken::ToString() {
    return "repeat";
}