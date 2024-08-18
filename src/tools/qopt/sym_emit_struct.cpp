#include "qopt.h"
#include <QSymbolToken.h>
#include <QScriptToken.h>
#include <QScriptSymbol.h>
#include <StructureSymbol.h>

StructureSymbol *ReadStructure(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end, StructureSymbol** out);

void emit_struct() {
    int depth = 1;
    bool in_name_mode = true;
    StructureSymbol *sym;
    printf("emit struct: %08x\n", g_Deopt.root_name_checksum);
    ReadStructure(g_Deopt.script_tokens.begin(), g_Deopt.script_tokens.end(), &sym);

    
    sym->SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(sym);
    g_Deopt.script_tokens.clear();
}