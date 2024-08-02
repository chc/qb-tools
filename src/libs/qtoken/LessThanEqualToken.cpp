#include "LessThanEqualToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

LessThanEqualToken::LessThanEqualToken() {

}
LessThanEqualToken::~LessThanEqualToken() {

}
EScriptToken LessThanEqualToken::GetType() {
    return ESCRIPTTOKEN_LESSTHANEQUAL;
}
void LessThanEqualToken::LoadParams(IStream *stream) {

}
std::string LessThanEqualToken::ToString() {
    return " <= ";
}