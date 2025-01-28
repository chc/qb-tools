#ifndef _ALLARGSTOKEN_H
#define _ALLARGSTOKEN_H
#include "QScriptToken.h"

class AllArgsToken : public QScriptToken {
    public:
        AllArgsToken() {

        }
        ~AllArgsToken() {
            
        }
        EScriptToken GetType(){
            return ESCRIPTTOKEN_KEYWORD_ALLARGS;
        }
        void LoadParams(IStream *stream) {
                m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
                return AppendSpaceToString ? "<...> " : "<...>";
        }
    private:
};
#endif //_ALLARGSTOKEN_H