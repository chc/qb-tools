#ifndef _VECTORTOKEN_H
#define _VECTORTOKEN_H
#include "QScriptToken.h"

class VectorToken : public QScriptToken {
    public:
        VectorToken() {

        }
        VectorToken(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {

        }
        ~VectorToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_VECTOR;
        }
        void LoadParams(IStream *stream) {
            m_x = stream->ReadFloat();
            m_y = stream->ReadFloat();
            m_z = stream->ReadFloat();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_VECTOR);
            stream->WriteFloat(m_x);
            stream->WriteFloat(m_y);
            stream->WriteFloat(m_z);
        }
        std::string ToString() {
            std::ostringstream ss;
            ss << "Vec(" << m_x << "," << m_y << "," << m_z << ")" << (AppendSpaceToString ? " " : "");
            return ss.str();
        }
        float GetX() { return m_x; }
        float GetY() { return m_y; }
        float GetZ() { return m_z; }
    private:
        float m_x;
        float m_y;
        float m_z;
};
#endif //_VECTORTOKEN_H