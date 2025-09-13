#ifndef _UNKNOWNTOKEN_H
#define _UNKNOWNTOKEN_H
#include "QScriptToken.h"

class UnknownToken : public QScriptToken {
    public:
        UnknownToken(uint8_t type) : m_type(type) {

        }
        ~UnknownToken() {
            
        }
        EScriptToken GetType() {
            return (EScriptToken)m_type;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            std::ostringstream ss;
            ss << "Unknown(" << (int)m_type << ")";
            return ss.str();
        }
    private:
        uint8_t m_type;
};
#endif //_UNKNOWNTOKEN_H