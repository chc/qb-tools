#include "EndScriptToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndScriptToken::EndScriptToken() {

}
EndScriptToken::~EndScriptToken() {

}
EScriptToken EndScriptToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ENDSCRIPT;
}
void EndScriptToken::LoadParams(IStream *stream) {

}
std::string EndScriptToken::ToString() {
    return "endscript";
}