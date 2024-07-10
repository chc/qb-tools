#ifndef _COLONTOKEN_H
#define _COLONTOKEN_H
#include "QScriptToken.h"

class ColonToken : public QScriptToken {
    public:
        ColonToken();
        ~ColonToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_COLONTOKEN_H