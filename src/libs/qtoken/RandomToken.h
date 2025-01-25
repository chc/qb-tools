#ifndef _RANDOMTOKEN_H
#define _RANDOMTOKEN_H
#include "QScriptToken.h"

class RandomToken : public QScriptToken {
    public:
        RandomToken();
        RandomToken(int num_items);
        ~RandomToken();
        virtual EScriptToken GetType();
        void LoadParams(IStream *stream);
        virtual std::string ToString();
        std::vector<TokenInjection> GetInjections();
        void Write(IStream *stream);
        uint32_t GetNumItems() { return m_num_items; }
        uint32_t GetRandomOffset(int idx) { return m_offsets[idx].offset; }
        void SetRandomOffset(int idx, uint32_t offset) { m_offsets[idx].offset = offset; }
        void SetWeight(int idx, uint32_t weight) {
            m_offsets[idx].weight = weight;
        }
        void Rewrite(IStream *stream);
        uint32_t GetEndOffset() { return m_end_offset; }
        uint32_t CalculateTokenOffset(uint32_t index) {
            uint32_t offset = sizeof(uint8_t) + sizeof(uint32_t)
#ifndef NO_RANDOM_WEIGHTS
                + (sizeof(uint16_t) * m_num_items) 
#endif
                + (sizeof(uint32_t) + (sizeof(uint32_t) * index));
            return offset;
        } 
        static bool is_random_token(EScriptToken type) {
            switch (type) {
            case ESCRIPTTOKEN_KEYWORD_RANDOM:
            case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
            case ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
                return true;
                break;
            }
            return false;
        }
    private:
        RandomOffset *m_offsets;
        uint32_t m_num_items;
        uint32_t m_end_offset;
        uint32_t m_total_size;

};
#endif //_RANDOMTOKEN_H