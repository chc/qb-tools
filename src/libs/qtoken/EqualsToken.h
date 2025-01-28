#ifndef _EQUALSTOKEN_H
#define _EQUALSTOKEN_H
#include "QScriptToken.h"

class EqualsToken : public QScriptToken {
    public:
        EqualsToken() {

        }
        ~EqualsToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_EQUALS;
        }
        void LoadParams(IStream *stream) {

        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_EQUALS);
        }
        std::string ToString() {
            return AppendSpaceToString ? "= " : "=";
        }
    private:
};
#endif //_EQUALSTOKEN_H