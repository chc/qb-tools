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
        const char* GetName();
        void SetChecksumName(ChecksumNameToken* token);
        std::string ToString();
        //std::vector<TokenInjection> GetInjections();
};
#endif //_ARGTOKEN_H