#ifndef _RANDOMTOKEN_H
#define _RANDOMTOKEN_H
#include "QScriptToken.h"

#include <sstream>
#include <cassert>

class RandomToken : public QScriptToken {
    public:
        RandomToken() : RandomToken(0) {

        }
        RandomToken(int num_items) {
            if (num_items > 0) {
                m_offsets = new RandomOffset[num_items];
                for (int i = 0; i < num_items; i++) {
                    m_offsets[i].weight = 1;
                }
            }
            else {
                m_offsets = nullptr;
            }
            
            m_num_items = num_items;
        }
        ~RandomToken() {
            if(m_offsets) {
                delete[] m_offsets;
            } 
        }
        virtual EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOM;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_num_items = stream->ReadUInt32();
            m_offsets = new RandomOffset[m_num_items];
            
            for(int i=0;i<m_num_items;i++) {
#ifndef NO_RANDOM_WEIGHTS
                uint16_t weight = stream->ReadUInt16();
                m_offsets[i].weight = weight;
#else
                m_offsets[i].weight = 1;
#endif
            }
            
            std::ostringstream ss;
            for(int i=0;i<m_num_items;i++) {
                uint32_t o = stream->ReadUInt32();
                m_offsets[i].offset = o;
            }
            m_end_offset = stream->GetOffset();
            m_total_size = m_end_offset - m_file_offset;
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(GetType());
            stream->WriteUInt32(m_num_items);
        #ifndef NO_RANDOM_WEIGHTS
            for(int i=0;i<m_num_items;i++) {
                stream->WriteUInt16(m_offsets[i].weight);
            }
        #endif

            for(int i=0;i<m_num_items;i++) {
                stream->WriteUInt32(m_offsets[i].offset);
            }
            m_end_offset = stream->GetOffset();
            m_total_size = m_end_offset - m_file_offset;
        }

        virtual std::string ToString() {
            return "Random(";
        }
        std::vector<TokenInjection> GetInjections() {
            std::vector<TokenInjection> injections;

            for(int i=0;i<m_num_items;i++) {
                std::ostringstream ss;
                TokenInjection injection;
                injection.use_next_jump_offset = false;
                injection.offset = m_offsets[i].offset + CalculateTokenOffset(i) - m_total_size;

                ss << "@";
                if(m_offsets[i].weight != 1) {
                    ss << "*" << m_offsets[i].weight;
                }
                injection.token = ss.str();
                injections.push_back(injection);
            }

            TokenInjection injection;
            injection.offset = 0;
            injection.use_next_jump_offset = true;
            injection.token = ")";
            injections.push_back(injection);
            return injections;
        }
        
        uint32_t GetNumItems() { return m_num_items; }
        uint32_t GetRandomOffset(int idx) { return m_offsets[idx].offset; }
        void SetRandomOffset(int idx, uint32_t offset) { m_offsets[idx].offset = offset; }
        void SetWeight(int idx, uint32_t weight) {
            m_offsets[idx].weight = weight;
        }
        void Rewrite(IStream *stream) {
            assert(m_file_offset);
            uint32_t cursor = stream->GetOffset();
            stream->SetCursor(m_file_offset);
            Write(stream);
            stream->SetCursor(cursor);
        }
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