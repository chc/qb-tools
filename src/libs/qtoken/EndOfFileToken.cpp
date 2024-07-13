#include "EndOfFileToken.h"
#include "IQStream.h"

#include <stdio.h>
EndOfFileToken::EndOfFileToken() {

}
EndOfFileToken::~EndOfFileToken() {

}
EScriptToken EndOfFileToken::GetType() {
    return ESCRIPTTOKEN_ENDOFFILE;
}
void EndOfFileToken::LoadParams(IStream *stream) {
}
std::string EndOfFileToken::ToString() {
    return "\n";
}