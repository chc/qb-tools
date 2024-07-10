#include "CaseToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

CaseToken::CaseToken() {

}
CaseToken::~CaseToken() {

}
EScriptToken CaseToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_CASE;
}
void CaseToken::LoadParams(IQStream *stream) {

}
std::string CaseToken::ToString() {
    return "case ";
}