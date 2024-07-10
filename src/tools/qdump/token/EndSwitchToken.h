#ifndef _ENDSWITCHTOKEN_H
#define _ENDSWITCHTOKEN_H
#include "QScriptToken.h"

class EndSwitchToken : public QScriptToken {
    public:
        EndSwitchToken();
        ~EndSwitchToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_ENDSWITCHTOKEN_H