#ifndef _CHECKSUMNAMETOKEN_H
#define _CHECKSUMNAMETOKEN_H
#include "QScriptToken.h"
#include <string>

class ChecksumNameToken : public QScriptToken {
    public:
        ChecksumNameToken() {
            m_name = NULL;
        }
        ChecksumNameToken(uint32_t checksum, const char *name) {
            m_checksum = checksum;
            m_name = strdup(name);
        }
        ~ChecksumNameToken() {
            if(m_name) {
                free((void *)m_name);    
            }
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_CHECKSUM_NAME;
        }
        void LoadParams(IStream *stream) {
            m_checksum = stream->ReadUInt32();
            m_name = stream->ReadNTS();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_CHECKSUM_NAME);
            stream->WriteUInt32(m_checksum);
            stream->WriteNTS(m_name);
        }
        uint32_t GetChecksum() {
            return m_checksum;
        }
        const char *GetName() {
            return m_name;
        }
        std::string ToString() {
            return "";
        }
    private:
        uint32_t m_checksum;
        const char *m_name;
};
#endif //_NAMETOKEN_H