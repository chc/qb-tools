#ifndef _ARGUMENTPACKTOKEN_H
#define _ARGUMENTPACKTOKEN_H
#include "QScriptToken.h"
#include <cassert>

class ArgumentPackToken : public QScriptToken {
    public:
        ArgumentPackToken() {
            m_ref_type = 0;
            m_is_required_params = false;
        }
        ~ArgumentPackToken() {
            
        }
        virtual EScriptToken GetType() {
            return ESCRIPTTOKEN_ARGUMENTPACK;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset()-sizeof(uint8_t);
        }

        //this is custom QToken serialiation, not actually used by the game
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_ARGUMENTPACK);
        }
        void LoadExtendedParams(IStream *stream) {
            m_ref_type = stream->ReadByte();
            m_is_required_params = stream->ReadByte();
        }
        void WriteExtendedParams(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_ARGUMENTPACK);
            stream->WriteByte(m_ref_type);
            stream->WriteByte(m_is_required_params);
        }
        virtual std::string ToString() {
            // this token is $ based on a printf in THP8
            // printf "UI WARNING: $ui_current_zone_index = %d ($ui_num_zones = %l)" d = $ui_current_zone_index l = $ui_num_zones
            
            // THDJ Wii: 4B is followed by an untokenized integer pointer into a dict
            std::string result = "$";
            if(m_is_required_params) {
                result += "req_";
            }

            //this does not seem to actually be linked to type, but since i have no idea i'll just go with it until i figure out what it is
            switch (m_ref_type) {
                case 0: 
                    result += "none$";
                break;
                case 1: //ESYMBOLTYPE_INTEGER
                    result += "int$";
                break;
                case 2: //ESYMBOLTYPE_FLOAT
                    result += "float$";
                break;
                case 3: //ESYMBOLTYPE_STRING
                    result += "string$";
                break;
                case 4: //ESYMBOLTYPE_LOCALSTRING
                    result += "localstring$";
                break;
                case 5: //ESYMBOLTYPE_PAIR
                    result += "pair$";
                break;
                case 6: //ESYMBOLTYPE_VECTOR
                    result += "vec$";
                break;
                case 7: //ESYMBOLTYPE_QSCRIPT
                    result += "script$";
                break;
                case 8: //ESYMBOLTYPE_CFUNCTION
                    result += "cfunc$";
                break;
                case 10: // ESYMBOLTYPE_STRUCTURE
                    result += "struct$";
                break;
                case 11: // ESYMBOLTYPE_STRUCTUREPOINTER
                    result += "ptr_struct$";
                break;
                case 12: //ESYMBOLTYPE_ARRAY
                    result += "array$";
                break;
                case 13: //ESYMBOLTYPE_NAME
                    result += "name$";
                break;
                default:
                    assert(false);
                break;
            }
            return result;
        }

        void SetRefType(uint8_t type) { m_ref_type = type; }
        void SetIsRequiredParams(bool required) { m_is_required_params =required; }
        uint8_t GetRefType() { return m_ref_type; }
        bool GetIsRequiredParams() { return m_is_required_params; }
    private:
        bool m_is_required_params;
        uint8_t m_ref_type;
};
#endif //_ARGUMENTPACKTOKEN_H