#ifndef _TYPEHINT_CONTROLLER_H
#define _TYPEHINT_CONTROLLER_H
#include "QScriptToken.h"

class TypeHint_Controller : public QScriptToken {
    public:
        TypeHint_Controller() {

        }
        ~TypeHint_Controller() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_TYPEHINT_CONTROLLER;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "TypeHint_Controller " : "TypeHint_Controller";
        }
    private:
};
#endif //_TYPEHINT_CONTROLLER_H