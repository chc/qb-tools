#ifndef _ENDOFLINENUMBERTOKEN_H
#define _ENDOFLINENUMBERTOKEN_H
#include "QScriptToken.h"

class EndOfLineNumberToken : public QScriptToken {
    public:
        EndOfLineNumberToken();
        ~EndOfLineNumberToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        uint32_t GetLineNumber();
        std::string ToString();
    private:
        uint32_t m_line_number;
};
#endif //_ENDOFLINENUMBERTOKEN_H