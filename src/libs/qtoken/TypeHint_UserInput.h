#ifndef _TYPEHINT_USERINPUT_H
#define _TYPEHINT_USERINPUT_H
#include "QScriptToken.h"

class TypeHint_UserInput : public QScriptToken {
    public:
        TypeHint_UserInput() {

        }
        ~TypeHint_UserInput() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_TYPEHINT_USERINPUT;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "TypeHint_UserInput " : "TypeHint_UserInput";
        }
    private:
};
#endif //_TYPEHINT_USERINPUT_H