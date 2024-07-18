#include "DefaultToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

DefaultToken::DefaultToken() {

}
DefaultToken::~DefaultToken() {

}
EScriptToken DefaultToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_DEFAULT;
}
void DefaultToken::LoadParams(IStream *stream) {

}
std::string DefaultToken::ToString() {
    return "default ";
}