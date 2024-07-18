#include "EndOfLineToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <iostream>
EndOfLineToken::EndOfLineToken() {

}
EndOfLineToken::~EndOfLineToken() {

}
EScriptToken EndOfLineToken::GetType() {
    return ESCRIPTTOKEN_ENDOFLINE;
}
void EndOfLineToken::LoadParams(IStream *stream) {
}
std::string EndOfLineToken::ToString() {
    return "\n";
}