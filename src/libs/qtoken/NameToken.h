#ifndef _NAMETOKEN_H
#define _NAMETOKEN_H
#include "QScriptToken.h"
#include "ChecksumNameToken.h"
#include <sstream>

class ChecksumNameToken;
class NameToken : public QScriptToken {
    public:
        NameToken() {
            m_checksum_name = NULL;
            m_checksum = 0;
        }
        NameToken(uint32_t v) {
            m_checksum_name = NULL;
            m_checksum = v;
        }
        ~NameToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_NAME;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset()-sizeof(uint8_t);
            m_checksum = stream->ReadUInt32();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_NAME);
            stream->WriteUInt32(m_checksum);
        }
        uint32_t GetChecksum() {
            return m_checksum;
        }
        const char *GetName() {
            if(m_checksum_name) {
                return m_checksum_name->GetName();
            }
            return nullptr;
        }
        void SetChecksumName(ChecksumNameToken *token) {
            m_checksum_name = token;
        }
        std::string ToString() {
            std::ostringstream ss;
            
            const char *name = GetName();
            if(name != nullptr) {
                bool abnormal = false;
                int i = 0;
                while (name[i] != '\0') {
                    //for (int i = 0; i < length; i++) {
                    if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') || name[i] == '_') {
                    } else {
                        if ((i > 0) && name[i] >= '0' && name[i] <= '9') {
                        } else {
                            abnormal = true;
                            break;
                        }
                    }
                    i++;
                }
                if (abnormal) {
                    ss << "#\"" << name << "\"" << (AppendSpaceToString ? " " : "");
                } else {
                    ss << name << (AppendSpaceToString ? " " : "");
                }
            } else {
                ss << "#\"0x" << std::hex << std::uppercase << m_checksum << "\"" << (AppendSpaceToString ? " " : "");
            }
            return ss.str();
        }
    private:
        uint32_t m_checksum;
        ChecksumNameToken *m_checksum_name;
};
#endif //_NAMETOKEN_H