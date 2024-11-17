#include "ArgumentPackToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cassert>


ArgumentPackToken::ArgumentPackToken() {

}
ArgumentPackToken::~ArgumentPackToken() {

}
EScriptToken ArgumentPackToken::GetType() {
    return ESCRIPTTOKEN_ARGUMENTPACK;
}
void ArgumentPackToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset()-sizeof(uint8_t);
}
void ArgumentPackToken::LoadExtendedParams(IStream *stream) {
    m_ref_type = stream->ReadByte();
    m_is_required_params = stream->ReadByte();
}
void ArgumentPackToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ARGUMENTPACK);
}
void ArgumentPackToken::WriteExtendedParams(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ARGUMENTPACK);
    stream->WriteByte(m_ref_type);
    stream->WriteByte(m_is_required_params);
}
std::string ArgumentPackToken::ToString() {
    // this token is $ based on a printf in THP8
    // printf "UI WARNING: $ui_current_zone_index = %d ($ui_num_zones = %l)" d = $ui_current_zone_index l = $ui_num_zones
    
    // THDJ Wii: 4B is followed by an untokenized integer pointer into a dict
    std::string result = "$";
    if(m_is_required_params) {
        result += "req_";
    }
    switch (m_ref_type) {
        case 0: //understand this more, qdeopt bug or maybe i'm forgetting something
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
        case 10: // ESYMBOLTYPE_STRUCTURE
            result += "struct$";
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