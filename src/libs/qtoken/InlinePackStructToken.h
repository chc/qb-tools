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
        std::string ToString();
        StructureSymbol GetValue() { return m_inner_struct; }
    private:
        StructureSymbol m_inner_struct;
};
#endif //_INLINEPACKSTRUCTTOKEN_H