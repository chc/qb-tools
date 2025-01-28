#ifndef _CASETOKEN_H
#define _CASETOKEN_H
#include "QScriptToken.h"

class CaseToken : public QScriptToken {
    public:
        CaseToken() {

        }
        ~CaseToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_CASE;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "case " : "case";
        }
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_CASETOKEN_H