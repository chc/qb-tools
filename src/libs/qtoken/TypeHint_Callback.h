#ifndef _TYPEHINT_CALLBACK_H
#define _TYPEHINT_CALLBACK_H
#include "QScriptToken.h"

class TypeHint_Callback : public QScriptToken {
    public:
        TypeHint_Callback() {

        }
        ~TypeHint_Callback() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_TYPEHINT_CALLBACK;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "TypeHint_Callback " : "TypeHint_Callback";
        }
    private:
};
#endif //_TYPEHINT_CALLBACK_H