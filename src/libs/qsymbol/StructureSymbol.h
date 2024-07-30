#ifndef _STRUCTURESYMBOL_H
#define _STRUCTURESYMBOL_H
#include "QSymbolToken.h"
#include <vector>

class StructureSymbol : public QSymbolToken {
    public:
        StructureSymbol();
        ~StructureSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsNoOffset(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        static QSymbolToken *NextSymbol(IStream *stream);
        std::vector<QSymbolToken *> GetTokens() { return m_children; }
        void SetTokens(std::vector<QSymbolToken *> c) { m_children = c;}
    private:
        std::vector<QSymbolToken *> m_children;
};
#endif //_STRUCTURESYMBOL_H