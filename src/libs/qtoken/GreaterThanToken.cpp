#include "GreaterThanToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

GreaterThanToken::GreaterThanToken() {

}
GreaterThanToken::~GreaterThanToken() {

}
EScriptToken GreaterThanToken::GetType() {
    return ESCRIPTTOKEN_GREATERTHAN;
}
void GreaterThanToken::LoadParams(IStream *stream) {

}
std::string GreaterThanToken::ToString() {
    return " > ";
}