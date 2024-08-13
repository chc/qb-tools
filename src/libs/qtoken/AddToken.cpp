#include "AddToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

AddToken::AddToken() {

}
AddToken::~AddToken() {

}
EScriptToken AddToken::GetType() {
    return ESCRIPTTOKEN_ADD;
}
void AddToken::LoadParams(IStream *stream) {

}
std::string AddToken::ToString() {
    return AppendSpaceToString ? "+ " : "+";
}