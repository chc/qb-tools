#ifndef _VECTORTOKEN_H
#define _VECTORTOKEN_H
#include "QScriptToken.h"

class VectorToken : public QScriptToken {
    public:
        VectorToken();
        VectorToken(float x, float y, float z);
        ~VectorToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        float GetX() { return m_x; }
        float GetY() { return m_y; }
        float GetZ() { return m_z; }
    private:
        float m_x;
        float m_y;
        float m_z;
};
#endif //_VECTORTOKEN_H