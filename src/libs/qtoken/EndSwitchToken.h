#ifndef _ENDSWITCHTOKEN_H
#define _ENDSWITCHTOKEN_H
#include "QScriptToken.h"

class EndSwitchToken : public QScriptToken {
    public:
        EndSwitchToken();
        ~EndSwitchToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -2; }
    private:
};
#endif //_ENDSWITCHTOKEN_H