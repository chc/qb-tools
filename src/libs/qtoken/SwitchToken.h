#ifndef _SWITCHTOKEN_H
#define _SWITCHTOKEN_H
#include "QScriptToken.h"

class SwitchToken : public QScriptToken {
    public:
        SwitchToken() {

        }
        ~SwitchToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_SWITCH;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "switch " : "switch";
        }
        int GetPostTabOffset() { return 2; }
    private:
};
#endif //_SWITCHTOKEN_H