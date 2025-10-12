#ifndef _QENGINE_H
#define _QENGINE_H
#include <map>
#include <QSymbol.h>
#include <ISStream.h>
#include <assert.h>

#include <QScriptSymbol.h>
#include <StructureSymbol.h>

#include <QScriptToken.h>

typedef bool(*QCFunc)(StructureSymbol*);

class QScript;
class QEngine {
    public:
        QEngine();
        void load_symbol(QSymbol *sym);
        bool run_script(uint32_t checksum, StructureSymbol *args = nullptr);
        void register_cfunc(uint32_t checksum, QCFunc callback);
    private:
        bool run_qscript(QScript *script, QScriptSymbol *sym, StructureSymbol *args = nullptr);


        std::map<uint32_t, QSymbol *> root_symbols;

        std::map<uint32_t, QCFunc> m_cfuncs;

};
#endif //_QENGINE_H