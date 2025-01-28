#ifndef _ARGTOKEN_H
#define _ARGTOKEN_H
#include "QScriptToken.h"

class ChecksumNameToken;
class ArgToken : public QScriptToken {
    public:
        ArgToken() {

        }
        ~ArgToken() {

        }
        EScriptToken GetType(){
            return ESCRIPTTOKEN_ARG;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        void Write(IStream* stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_ARG);
        }
        std::string ToString() {
            return "&";
        }

        //std::vector<TokenInjection> ArgToken::GetInjections() {
        //    std::vector<TokenInjection> result;
        //    TokenInjection injection;
        //    injection.use_next_jump_offset = false;
        //    injection.offset = sizeof(uint32_t) + sizeof(uint8_t); //size of name token
        //    injection.token = "> ";
        //    result.push_back(injection);
        //    return result;
        //}
};
#endif //_ARGTOKEN_H