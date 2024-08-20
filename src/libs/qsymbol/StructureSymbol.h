#ifndef _STRUCTURESYMBOL_H
#define _STRUCTURESYMBOL_H
#include "QSymbolToken.h"
#include <vector>

class StructureSymbol : public QSymbol {
    public:
        StructureSymbol();
        StructureSymbol(std::vector<QSymbol *> children);
        ~StructureSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsNoOffset(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        static QSymbol *NextSymbol(IStream *stream);
        std::vector<QSymbol *> GetTokens() { return m_children; }
        void SetTokens(std::vector<QSymbol *> c) { m_children = c;}
        void Write(IStream *stream);
        void WriteNoOffset(IStream *stream);
        void WriteToArray(IStream *stream);
        void WriteSymbol(IStream *stream, QSymbol *sym);
    private:
        std::vector<QSymbol *> m_children;
};
#endif //_STRUCTURESYMBOL_H