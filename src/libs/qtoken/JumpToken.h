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
        uint32_t GetOffset();
    private:
        uint32_t m_offset;
};
#endif //_JUMPTOKEN_H