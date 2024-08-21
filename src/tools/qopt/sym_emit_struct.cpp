#include "qopt.h"
#include <QSymbol.h>
#include <QScriptToken.h>
#include <QScriptSymbol.h>
#include <StructureSymbol.h>

std::vector<QScriptToken *>::iterator ReadStructure(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end, StructureSymbol** out);

void emit_struct() {
    int depth = 1;
    bool in_name_mode = true;
    StructureSymbol *sym;
    ReadStructure(g_QOpt.script_tokens.begin(), g_QOpt.script_tokens.end(), &sym);
    
    sym->SetNameChecksum(g_QOpt.root_name_checksum);
    g_QOpt.write_stream->WriteSymbol(sym);
    g_QOpt.script_tokens.clear();
}