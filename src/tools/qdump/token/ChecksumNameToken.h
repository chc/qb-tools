#ifndef _CHECKSUMNAMETOKEN_H
#define _CHECKSUMNAMETOKEN_H
#include "QScriptToken.h"
#include <string>

class ChecksumNameToken : public QScriptToken {
    public:
        ChecksumNameToken();
        ~ChecksumNameToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        uint32_t GetChecksum();
        const char *GetName();
        std::string ToString();
    private:
        uint32_t m_checksum;
        const char *m_name;
};
#endif //_NAMETOKEN_H