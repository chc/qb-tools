#ifndef _PAIRTOKEN_H
#define _PAIRTOKEN_H
#include "QScriptToken.h"

class PairToken : public QScriptToken {
    public:
        PairToken() : PairToken(0, 0) {

        }

        PairToken(float x, float y) : m_x(x), m_y(y) {

        }

        ~PairToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_PAIR;
        }
        void LoadParams(IStream *stream) {
            m_x = stream->ReadFloat();
            m_y = stream->ReadFloat();
        }
        void Write(IStream *stream)  {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_PAIR);
            stream->WriteFloat(m_x);
            stream->WriteFloat(m_y);
        }
        std::string ToString()  {
            std::ostringstream ss;
            ss << "Pair(" << m_x << "," << m_y << ")" << (AppendSpaceToString ? " " : "");
            return ss.str();
        }
        float GetX() { return m_x; }
        float GetY() { return m_y; }
    private:
        float m_x;
        float m_y;
};
#endif //_PAIRTOKEN_H