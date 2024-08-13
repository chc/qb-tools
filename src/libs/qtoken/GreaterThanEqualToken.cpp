#include "GreaterThanEqualToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

GreaterThanEqualToken::GreaterThanEqualToken() {

}
GreaterThanEqualToken::~GreaterThanEqualToken() {

}
EScriptToken GreaterThanEqualToken::GetType() {
    return ESCRIPTTOKEN_GREATERTHANEQUAL;
}
void GreaterThanEqualToken::LoadParams(IStream *stream) {

}
std::string GreaterThanEqualToken::ToString() {
    return AppendSpaceToString ? ">= " : ">=";
}