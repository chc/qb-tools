#ifndef _WIDESTRINGTOKEN_H
#define _WIDESTRINGTOKEN_H
#include "QScriptToken.h"

class WideStringToken : public QScriptToken {
    public:
        WideStringToken(std::u16string value);
        WideStringToken();
        ~WideStringToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
    private:
        std::u16string m_value;
};
#endif //_WIDESTRINGTOKEN_H