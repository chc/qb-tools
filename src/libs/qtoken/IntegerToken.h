#ifndef _INTEGERTOKEN_H
#define _INTEGERTOKEN_H
#include "QScriptToken.h"
#include <sstream>

class IntegerToken : public QScriptToken {
    public:
        IntegerToken() : IntegerToken(0) {

        }
        IntegerToken(int32_t v) {
            m_value = v;
        }
        ~IntegerToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_INTEGER;
        }
        void LoadParams(IStream *stream) {
            m_value = stream->ReadInt32();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_INTEGER);
            stream->WriteInt32(m_value);
        }
        std::string ToString() {
            std::ostringstream ss;
            ss << m_value << (AppendSpaceToString ? " " : "");
            return ss.str();
        }
        int32_t GetValue() { return m_value; }
    private:
        int32_t m_value;
};
#endif //_INTEGERTOKEN_H