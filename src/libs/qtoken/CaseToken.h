#ifndef _CASETOKEN_H
#define _CASETOKEN_H
#include "QScriptToken.h"

class CaseToken : public QScriptToken {
    public:
        CaseToken();
        ~CaseToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_CASETOKEN_H