#ifndef _ARGUMENTPACKTOKEN_H
#define _ARGUMENTPACKTOKEN_H
#include "QScriptToken.h"


class ArgumentPackToken : public QScriptToken {
    public:
        ArgumentPackToken();
        ~ArgumentPackToken();
        virtual EScriptToken GetType();
        void LoadParams(IStream *stream);

        //this is custom QToken serialiation, not actually used by the game
        void Write(IStream *stream);
        void LoadExtendedParams(IStream *stream);
        void WriteExtendedParams(IStream *stream);
        virtual std::string ToString();

        void SetRefType(uint8_t type) { m_ref_type = type; }
        void SetIsRequiredParams(bool required) { m_is_required_params =required; }
        uint8_t GetRefType() { return m_ref_type; }
        bool GetIsRequiredParams() { return m_is_required_params; }
    private:
        bool m_is_required_params;
        uint8_t m_ref_type;
};
#endif //_ARGUMENTPACKTOKEN_H