#include "EndArrayToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndArrayToken::EndArrayToken() {

}
EndArrayToken::~EndArrayToken() {

}
EScriptToken EndArrayToken::GetType() {
    return ESCRIPTTOKEN_ENDARRAY;
}
void EndArrayToken::LoadParams(IStream *stream) {

}
std::string EndArrayToken::ToString() {
    return "] ";
}