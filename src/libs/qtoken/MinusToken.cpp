#include "MinusToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

MinusToken::MinusToken() {

}
MinusToken::~MinusToken() {

}
EScriptToken MinusToken::GetType() {
    return ESCRIPTTOKEN_MINUS;
}
void MinusToken::LoadParams(IStream *stream) {

}
std::string MinusToken::ToString() {
    return " - ";
}