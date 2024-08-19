
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <QScriptToken.h>
#include <cassert>

extern std::vector<QScriptToken *> g_tokens;
QScriptToken *find_by_offset(size_t offset);
void dump_token_offsets(QScriptToken *token);