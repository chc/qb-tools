#ifndef _WIDESTRINGTOKEN_H
#define _WIDESTRINGTOKEN_H
#include "QScriptToken.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

#include <iostream>
#include <codecvt>

class WideStringToken : public QScriptToken {
    public:
        WideStringToken(std::u16string value) : m_value(value) {

        }
        WideStringToken() {

        }
        ~WideStringToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_WIDESTRING;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            uint32_t len = stream->ReadUInt32();

#ifdef ISTREAM_SYMBOL_ENDIAN
            uint8_t mode = stream->GetReadEndianMode();
            stream->SetReadEndian(ISTREAM_SYMBOL_ENDIAN);
#endif


            while(len) {
                uint16_t c = stream->ReadUInt16();
                if(c != 0) {
                    m_value += c;
                }
                
                len -= 2;
            }

#ifdef ISTREAM_SYMBOL_ENDIAN
            stream->SetReadEndian(mode);
#endif
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            
            stream->WriteByte(ESCRIPTTOKEN_WIDESTRING);
            uint32_t len = m_value.length();
            len++; //include null byte
            stream->WriteUInt32(len * 2);

#ifdef ISTREAM_SYMBOL_ENDIAN
            uint8_t mode = stream->GetWriteEndianMode();
            stream->SetWriteEndian(ISTREAM_SYMBOL_ENDIAN);
#endif

            for(int i=0;i<len;i++) {
                stream->WriteUInt16(m_value[i]);
            }
#ifdef ISTREAM_SYMBOL_ENDIAN
            stream->SetWriteEndian(mode);
#endif
        }
        std::string ToString() {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
            std::string v = converter.to_bytes(m_value);

            std::ostringstream ss;
            ss << "$'" << v << "'" << (AppendSpaceToString ? " " : "");
            return ss.str();
        }
    private:
        std::u16string m_value;
};
#endif //_WIDESTRINGTOKEN_H