#ifndef _SHORTJUMPTOKEN_H
#define _SHORTJUMPTOKEN_H
#include "QScriptToken.h"

class ShortJumpToken : public QScriptToken {
    public:
        ShortJumpToken();
        ~ShortJumpToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff);
    private:
        uint32_t m_offset;
};
#endif //_SHORTJUMPTOKEN_H