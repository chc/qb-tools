#include "ColonToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ColonToken::ColonToken() {

}
ColonToken::~ColonToken() {

}
EScriptToken ColonToken::GetType() {
    return ESCRIPTTOKEN_COLON;
}
void ColonToken::LoadParams(IStream *stream) {

}
std::string ColonToken::ToString() {
    return ":";
}