#include "RepeatToken.h"
#include "IQStream.h"

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
void RepeatToken::LoadParams(IStream *stream) {

}
std::string RepeatToken::ToString() {
    return AppendSpaceToString ? "repeat " : "repeat";
}