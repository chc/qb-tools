#ifndef _ENDSWITCHTOKEN_H
#define _ENDSWITCHTOKEN_H
#include "QScriptToken.h"

class EndSwitchToken : public QScriptToken {
    public:
        EndSwitchToken() {

        }
        ~EndSwitchToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_ENDSWITCH;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "endswitch";
        }
        int GetPreTabOffset() { return -2; }
    private:
};
#endif //_ENDSWITCHTOKEN_H