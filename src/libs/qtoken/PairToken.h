#ifndef _PAIRTOKEN_H
#define _PAIRTOKEN_H
#include "QScriptToken.h"

class PairToken : public QScriptToken {
    public:
        PairToken();
        ~PairToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        float m_values[2];
};
#endif //_PAIRTOKEN_H