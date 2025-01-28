#ifndef _STRINGTOKEN_H
#define _STRINGTOKEN_H
#include "QScriptToken.h"

class StringToken : public QScriptToken {
    public:
        StringToken() {

        }
        StringToken(std::string value) : m_value(value) {

        }
        ~StringToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_STRING;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            uint32_t length = stream->ReadUInt32();
            while(length--) {
                char ch = stream->ReadByte();
                if(ch == 0)
                    break;
                m_value += ch;
            }
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_STRING);
            stream->WriteUInt32(m_value.length()+1);
            stream->WriteNTS(m_value.c_str());
        }
        std::string ToString() {
            std::ostringstream ss;
            
            // fix this to place backslash before " and before backslash
            
            ss << "\"" << m_value << "\"" << (AppendSpaceToString ? " " : "");
            return ss.str();
        }
        std::string GetValue() { return m_value; }
    private:
        std::string m_value;
};
#endif //_STRINGTOKEN_H