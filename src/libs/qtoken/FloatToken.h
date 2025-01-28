#ifndef _FLOATTOKEN_H
#define _FLOATTOKEN_H
#include "QScriptToken.h"
#include <sstream>
#include <iomanip>
class FloatToken : public QScriptToken {
    public:
        FloatToken() : FloatToken(0.0) {
        }
        FloatToken(float v) {
            m_value = v;
        }
        ~FloatToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_FLOAT;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_value = stream->ReadFloat();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_FLOAT);
            stream->WriteFloat(m_value);
        }
        std::string ToString() {
            std::ostringstream ss;
            ss << std::setprecision(2);
            ss << std::showpoint;
            ss << std::fixed;
            ss << m_value;
            if (AppendSpaceToString) {
                ss << " ";
            }
            return ss.str();
        }
        float GetValue() { return m_value; }
    private:
        float m_value;
};
#endif //_FLOATTOKEN_H