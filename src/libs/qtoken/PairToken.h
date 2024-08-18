#ifndef _PAIRTOKEN_H
#define _PAIRTOKEN_H
#include "QScriptToken.h"

class PairToken : public QScriptToken {
    public:
        PairToken();
        PairToken(float x, float y);
        ~PairToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        float GetX() { return m_x; }
        float GetY() { return m_y; }
    private:
        float m_x;
        float m_y;
};
#endif //_PAIRTOKEN_H