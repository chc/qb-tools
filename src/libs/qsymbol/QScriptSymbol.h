#ifndef _QSCRIPTSYMBOL_H
#define _QSCRIPTSYMBOL_H
#include "QSymbolToken.h"

class QScriptSymbol : public QSymbolToken {
    public:
        QScriptSymbol();
        ~QScriptSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_QSCRIPTSYMBOL_H