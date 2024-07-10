#include "RandomRangeToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomRangeToken::RandomRangeToken() {

}
RandomRangeToken::~RandomRangeToken() {

}
EScriptToken RandomRangeToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE;
}
void RandomRangeToken::LoadParams(IQStream *stream) {
}
std::string RandomRangeToken::ToString() {
    return "RandomRange";
}