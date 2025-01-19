#ifndef _ENDOFLINENUMBERTOKEN_H
#define _ENDOFLINENUMBERTOKEN_H
#include "QScriptToken.h"

class EndOfLineNumberToken : public QScriptToken {
    public:
        EndOfLineNumberToken();
        EndOfLineNumberToken(uint32_t line_number);
        ~EndOfLineNumberToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream* stream);
        uint32_t GetLineNumber();
        std::string ToString();
    private:
        uint32_t m_line_number;
};
#endif //_ENDOFLINENUMBERTOKEN_H