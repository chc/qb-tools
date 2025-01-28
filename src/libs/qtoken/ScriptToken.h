#ifndef _SCRIPTTOKEN_H
#define _SCRIPTTOKEN_H
#include "QScriptToken.h"

class ScriptToken : public QScriptToken {
    public:
        ScriptToken() {

        }
        ~ScriptToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_SCRIPT;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return "script ";
        }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_SCRIPTTOKEN_H