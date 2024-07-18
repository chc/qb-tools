#include "EndIfToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndIfToken::EndIfToken() {

}
EndIfToken::~EndIfToken() {

}
EScriptToken EndIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ENDIF;
}
void EndIfToken::LoadParams(IStream *stream) {

}
std::string EndIfToken::ToString() {
    return "endif";
}
int EndIfToken::GetPreTabOffset() {
    return -1;
}