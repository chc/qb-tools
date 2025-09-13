#ifndef _TYPEHINT_ID_H
#define _TYPEHINT_ID_H
#include "QScriptToken.h"

class TypeHint_ID : public QScriptToken {
    public:
        TypeHint_ID() {

        }
        ~TypeHint_ID() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_TYPEHINT_ID;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "TypeHint_ID " : "TypeHint_ID";
        }
    private:
};
#endif //_TYPEHINT_ID_H