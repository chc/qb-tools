#include "AndToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

AndToken::AndToken() {

}
AndToken::~AndToken() {

}
EScriptToken AndToken::GetType() {
    return ESCRIPTTOKEN_AND;
}
void AndToken::LoadParams(IStream *stream) {

}
std::string AndToken::ToString() {
    return AppendSpaceToString ? "AND " : "AND";
}