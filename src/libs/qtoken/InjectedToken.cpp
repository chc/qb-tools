#include "InjectedToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

InjectedToken::InjectedToken(std::string token) {
    m_token = token;
}
InjectedToken::~InjectedToken() {

}
EScriptToken InjectedToken::GetType() {
    return (EScriptToken)-1;
}
void InjectedToken::LoadParams(IStream *stream) {

}
std::string InjectedToken::ToString() {
    return AppendSpaceToString ? (m_token + " ") : m_token;
}