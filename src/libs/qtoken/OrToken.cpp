#include "OrToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

OrToken::OrToken() {

}
OrToken::~OrToken() {

}
EScriptToken OrToken::GetType() {
    return ESCRIPTTOKEN_OR;
}
void OrToken::LoadParams(IStream *stream) {

}
std::string OrToken::ToString() {
    return " || ";
}