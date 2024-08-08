#ifndef _JUMPTOKEN_H
#define _JUMPTOKEN_H
#include "QScriptToken.h"

class JumpToken : public QScriptToken {
    public:
        JumpToken();
        ~JumpToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff);
    private:
        uint32_t m_offset;
};
#endif //_JUMPTOKEN_H