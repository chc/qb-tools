#include "ScriptToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>


int32_t g_last_script_keyword = -1; ///XXX: why is -1 init needed? figure out problems with inline pack offsets...
int32_t g_last_script_keyword_write = 0;
ScriptToken::ScriptToken() {

}
ScriptToken::~ScriptToken() {

}
EScriptToken ScriptToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_SCRIPT;
}
void ScriptToken::LoadParams(IStream *stream) {

}
std::string ScriptToken::ToString() {
    return "script ";
}
int	ScriptToken::GetPostTabOffset() {
    return 1;
}