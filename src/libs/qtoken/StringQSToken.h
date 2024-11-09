#ifndef _STRINGQSTOKEN_H
#define _STRINGQSTOKEN_H
#include "QScriptToken.h"

class StringQSToken : public QScriptToken {
    public:
        StringQSToken();
        StringQSToken(uint32_t checksum);
        ~StringQSToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        uint32_t GetValue() { return m_checksum; }
    private:
        uint32_t m_checksum;
};
#endif //_STRINGQSTOKEN_H