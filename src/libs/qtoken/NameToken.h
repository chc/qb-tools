#ifndef _NAMETOKEN_H
#define _NAMETOKEN_H
#include "QScriptToken.h"

class ChecksumNameToken;
class NameToken : public QScriptToken {
    public:
        NameToken();
        NameToken(uint32_t v);
        ~NameToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        uint32_t GetChecksum();
        const char *GetName();
        void SetChecksumName(ChecksumNameToken *token);
        std::string ToString();
    private:
        uint32_t m_checksum;
        ChecksumNameToken *m_checksum_name;
};
#endif //_NAMETOKEN_H