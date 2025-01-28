#ifndef _RANDOMRANGETOKEN_H
#define _RANDOMRANGETOKEN_H
#include "QScriptToken.h"

class RandomRangeToken : public QScriptToken {
    public:
        RandomRangeToken() {

        }
        ~RandomRangeToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return "RandomRange ";
        }
    private:
};
#endif //_RANDOMRANGETOKEN_H