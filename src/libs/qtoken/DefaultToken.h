#ifndef _DEFAULTTOKEN_H
#define _DEFAULTTOKEN_H
#include "QScriptToken.h"

class DefaultToken : public QScriptToken {
    public:
        DefaultToken() {

        }
        ~DefaultToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_DEFAULT;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "default " : "default";
        }
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_DEFAULTTOKEN_H