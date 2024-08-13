#ifndef _SCRIPTTOKEN_H
#define _SCRIPTTOKEN_H
#include "QScriptToken.h"

class ScriptToken : public QScriptToken {
    public:
        ScriptToken();
        ~ScriptToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_SCRIPTTOKEN_H