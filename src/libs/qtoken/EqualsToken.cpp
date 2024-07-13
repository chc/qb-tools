#include "EqualsToken.h"
#include "IQStream.h"

#include <stdio.h>
EqualsToken::EqualsToken() {

}
EqualsToken::~EqualsToken() {

}
EScriptToken EqualsToken::GetType() {
    return ESCRIPTTOKEN_EQUALS;
}
void EqualsToken::LoadParams(IStream *stream) {
}
std::string EqualsToken::ToString() {
    return "= ";
}