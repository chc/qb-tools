#ifndef _RANDOMINTEGERTOKEN_H
#define _RANDOMINTEGERTOKEN_H
#include "QScriptToken.h"

class RandomIntegerToken : public QScriptToken {
    public:
        RandomIntegerToken() {

        }
        ~RandomIntegerToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER;
        }
        void LoadParams(IStream *stream) {

        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER);
        }
        std::string ToString() {
            return "RandomInt ";
        }
    private:
};
#endif //_RANDOMINTEGERTOKEN_H