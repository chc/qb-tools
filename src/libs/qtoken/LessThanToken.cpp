#include "LessThanToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

LessThanToken::LessThanToken() {

}
LessThanToken::~LessThanToken() {

}
EScriptToken LessThanToken::GetType() {
    return ESCRIPTTOKEN_LESSTHAN;
}
void LessThanToken::LoadParams(IStream *stream) {

}
std::string LessThanToken::ToString() {
    return " < ";
}