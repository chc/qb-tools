#ifndef _SWITCHTOKEN_H
#define _SWITCHTOKEN_H
#include "QScriptToken.h"

class SwitchToken : public QScriptToken {
    public:
        SwitchToken();
        ~SwitchToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_SWITCHTOKEN_H