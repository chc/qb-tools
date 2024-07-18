#ifndef _ALLARGSTOKEN_H
#define _ALLARGSTOKEN_H
#include "QScriptToken.h"

class AllArgsToken : public QScriptToken {
    public:
        AllArgsToken();
        ~AllArgsToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ALLARGSTOKEN_H