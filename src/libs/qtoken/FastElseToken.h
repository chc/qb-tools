#ifndef _FASTELSETOKEN_H
#define _FASTELSETOKEN_H
#include "QScriptToken.h"

class FastElseToken : public QScriptToken {
    public:
        FastElseToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        int GetPreTabOffset();
        int GetPostTabOffset();
        std::vector<TokenInjection> GetInjections();
    private:
        uint16_t m_offset;
};
#endif //_FASTELSETOKEN_H