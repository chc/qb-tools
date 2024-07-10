#include "DotToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

DotToken::DotToken() {

}
DotToken::~DotToken() {

}
EScriptToken DotToken::GetType() {
    return ESCRIPTTOKEN_DOT;
}
void DotToken::LoadParams(IQStream *stream) {

}
std::string DotToken::ToString() {
    return " . ";
}