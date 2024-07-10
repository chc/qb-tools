#ifndef _SCRIPTTOKEN_H
#define _SCRIPTTOKEN_H
#include "QScriptToken.h"

class ScriptToken : public QScriptToken {
    public:
        ScriptToken();
        ~ScriptToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
        int GetPostTabOffset();
    private:
};
#endif //_SCRIPTTOKEN_H