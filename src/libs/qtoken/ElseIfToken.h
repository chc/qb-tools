#ifndef _ELSEIFTOKEN_H
#define _ELSEIFTOKEN_H
#include "QScriptToken.h"

class ElseIfToken : public QScriptToken {
    public:
        ElseIfToken();
        ~ElseIfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset();
        int GetPostTabOffset();
        void Write(IStream *stream);

        uint16_t GetNextOffset() { return m_next_offset; }
        uint16_t GetEndIfOffset() {return m_endif_offset; }
        void SetNextOffset(IStream *stream, uint16_t offset);
        void SetEndIfOffset(IStream *stream, uint16_t offset);
        std::vector<TokenInjection> GetInjections();

    private:
        uint16_t m_next_offset;
        uint16_t m_endif_offset;
};
#endif //_ELSEIFTOKEN_H