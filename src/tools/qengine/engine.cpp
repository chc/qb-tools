#include "engine.h"
#include "script.h"
#include <MemoryStream.h>

#include <NameToken.h>
QEngine::QEngine() {

}
void QEngine::register_cfunc(uint32_t checksum, bool(*callback)(StructureSymbol*)) {
    m_cfuncs[checksum] = callback;
}
void QEngine::load_symbol(QSymbol *sym) {
    root_symbols[sym->GetNameChecksum()] = sym;
}
bool QEngine::run_script(uint32_t checksum, StructureSymbol *args) {
    QScript *script = new QScript(this);
    bool result = false;
    if(m_cfuncs.find(checksum) != m_cfuncs.end()) {
        result = m_cfuncs[checksum](args);
        delete script;
        return result;
    }
    QSymbol *sym = this->root_symbols[checksum];
    assert(sym);

    switch(sym->GetType()) {
        case ESYMBOLTYPE_QSCRIPT:
            result = run_qscript(script, reinterpret_cast<QScriptSymbol*>(sym), args);
        break;
        case ESYMBOLTYPE_CFUNCTION:
        case ESYMBOLTYPE_MEMBERFUNCTION:
        default:
            assert(false);
    }

    delete script;

    return result;
}

bool QEngine::run_qscript(QScript *script, QScriptSymbol *qscript, StructureSymbol *args) {
    MemoryStream ms(qscript->GetDecompBuff(), qscript->GetDecompLen());
    while(qscript->GetDecompLen() > ms.GetOffset()) {
        uint32_t read_offset = ms.GetOffset(); //skip type(1) + keyword script byte(1), name type(1)+value(4)
        uint8_t type = ms.ReadByte();
        QScriptToken *token = QScriptToken::Resolve(type);
        if(!token) {
            assert(false);
            break;
        }
        token->SetFileOffset(ms.GetOffset()-1);
        token->LoadParams(&ms);

        script->evaluate(token);
        
        //delete token; //figure out delete later... currently can't delete due to operator evaluation: (1+1)
    }
    return true;
}
