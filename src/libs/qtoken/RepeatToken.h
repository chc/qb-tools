#ifndef _REPEATTOKEN_H
#define _REPEATTOKEN_H
#include "QScriptToken.h"

class RepeatToken : public QScriptToken {
    public:
        RepeatToken() {

        }
        ~RepeatToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_REPEAT;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "repeat " : "repeat";
        }
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_REPEATTOKEN_H