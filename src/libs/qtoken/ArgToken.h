#ifndef _ARGTOKEN_H
#define _ARGTOKEN_H
#include "QScriptToken.h"

class ChecksumNameToken;
class ArgToken : public QScriptToken {
    public:
        ArgToken();
        ArgToken(uint32_t v);
        ~ArgToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream* stream);
        uint32_t GetChecksum();
        const char* GetName();
        void SetChecksumName(ChecksumNameToken* token);
        std::string ToString();
        //std::vector<TokenInjection> GetInjections();
    private:
        uint32_t m_checksum;
        ChecksumNameToken* m_checksum_name;
};
#endif //_ARGTOKEN_H