#ifndef _RANDOMTOKEN_H
#define _RANDOMTOKEN_H
#include "QScriptToken.h"


class RandomToken : public QScriptToken {
    public:
        RandomToken();
        ~RandomToken();
        virtual EScriptToken GetType();
        void LoadParams(IStream *stream);
        virtual std::string ToString();
        std::vector<TokenInjection> GetInjections();
        void Write(IStream *stream);
        uint32_t GetNumItems() { return m_num_items; }
        uint32_t GetRandomOffset(int idx) { return m_offsets[idx].offset; }
        void SetRandomOffset(int idx, uint32_t offset) { m_offsets[idx].offset = offset; }
        uint32_t GetFileOffset() { return m_file_offset; }
        uint32_t GetEndOffset() { return m_end_offset; }
        uint32_t CalculateTokenOffset(uint32_t index) {
            uint32_t offset = sizeof(uint8_t) + sizeof(uint32_t) + (sizeof(uint16_t) * m_num_items) + (sizeof(uint32_t) + (sizeof(uint32_t) * index));
            return offset;
        } 
    private:
        RandomOffset *m_offsets;
        uint32_t m_num_items;
        uint32_t m_file_offset;
        uint32_t m_end_offset;
        uint32_t m_total_size;
};
#endif //_RANDOMTOKEN_H