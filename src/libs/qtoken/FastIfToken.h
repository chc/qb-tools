#ifndef _FASTIFTOKEN_H
#define _FASTIFTOKEN_H
#include "QScriptToken.h"

class FastIfToken : public QScriptToken {
    public:
        FastIfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        int GetPostTabOffset();
        std::vector<TokenInjection> GetInjections();
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff);
    private:
        uint16_t m_offset;
};
#endif //_FASTIFTOKEN_H