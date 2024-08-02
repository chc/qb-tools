#ifndef _SHORTJUMPTOKEN_H
#define _SHORTJUMPTOKEN_H
#include "QScriptToken.h"

class ShortJumpToken : public QScriptToken {
    public:
        ShortJumpToken();
        ~ShortJumpToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_SHORTJUMPTOKEN_H