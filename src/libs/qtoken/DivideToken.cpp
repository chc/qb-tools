#include "DivideToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

DivideToken::DivideToken() {

}
DivideToken::~DivideToken() {

}
EScriptToken DivideToken::GetType() {
    return ESCRIPTTOKEN_DIVIDE;
}
void DivideToken::LoadParams(IStream *stream) {

}
std::string DivideToken::ToString() {
    return AppendSpaceToString ? "/ " : "/";
}