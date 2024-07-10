#ifndef _VECTORTOKEN_H
#define _VECTORTOKEN_H
#include "QScriptToken.h"

class VectorToken : public QScriptToken {
    public:
        VectorToken();
        ~VectorToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
        float m_values[3];
};
#endif //_VECTORTOKEN_H