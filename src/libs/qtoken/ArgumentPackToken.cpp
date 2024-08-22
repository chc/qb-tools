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
    //assert(false);
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
    return "$";
}