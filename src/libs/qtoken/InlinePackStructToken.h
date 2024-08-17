#ifndef _INLINEPACKSTRUCTTOKEN_H
#define _INLINEPACKSTRUCTTOKEN_H
#include "QScriptToken.h"

#include <StructureSymbol.h>

class InlinePackStructToken : public QScriptToken {
    public:
        InlinePackStructToken();
        ~InlinePackStructToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        void WriteStructure(IStream* stream, StructureSymbol *symbol);
        std::string ToString();
        StructureSymbol *GetValue() { return m_inner_struct; }
        void SetValue(StructureSymbol *sym);
        void SetPadding(int padding) { m_padding = padding; }
    private:
        StructureSymbol *m_inner_struct;
        int m_padding;
};
#endif //_INLINEPACKSTRUCTTOKEN_H