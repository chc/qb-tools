#ifndef _RANDOMFLOATTOKEN_H
#define _RANDOMFLOATTOKEN_H
#include "QScriptToken.h"

class RandomFloatToken : public QScriptToken {
    public:
        RandomFloatToken() {

        }
        ~RandomFloatToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT;
        }
        void LoadParams(IStream *stream) {

        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT);
        }
        std::string ToString() {
            return "RandomFloat ";
        }
    private:
};
#endif //_RANDOMFLOATTOKEN_H