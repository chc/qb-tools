#include "CommaToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

CommaToken::CommaToken() {

}
CommaToken::~CommaToken() {

}
EScriptToken CommaToken::GetType() {
    return ESCRIPTTOKEN_COMMA;
}
void CommaToken::LoadParams(IQStream *stream) {

}
std::string CommaToken::ToString() {
    return ",";
}