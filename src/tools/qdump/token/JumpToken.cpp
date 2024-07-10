#include "JumpToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

JumpToken::JumpToken() {

}
JumpToken::~JumpToken() {

}
EScriptToken JumpToken::GetType() {
    return ESCRIPTTOKEN_JUMP;
}
void JumpToken::LoadParams(IQStream *stream) {
    uint32_t offset = stream->ReadUInt32();
}
std::string JumpToken::ToString() {
    //return " JUMP ";
    return "";
}