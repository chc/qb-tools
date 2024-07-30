#include "ArgumentPackToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ArgumentPackToken::ArgumentPackToken() {

}
ArgumentPackToken::~ArgumentPackToken() {

}
EScriptToken ArgumentPackToken::GetType() {
    return ESCRIPTTOKEN_ARGUMENTPACK;
}
void ArgumentPackToken::LoadParams(IStream *stream) {

}
std::string ArgumentPackToken::ToString() {
    return " &";
}