#include "EndStructToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndStructToken::EndStructToken() {

}
EndStructToken::~EndStructToken() {

}
EScriptToken EndStructToken::GetType() {
    return ESCRIPTTOKEN_ENDSTRUCT;
}
void EndStructToken::LoadParams(IQStream *stream) {

}
std::string EndStructToken::ToString() {
    return "} ";
}