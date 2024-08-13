#include "ArgumentPackToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ArgumentPackToken::ArgumentPackToken() {

}
ArgumentPackToken::~ArgumentPackToken() {

}
EScriptToken ArgumentPackToken::GetType() {
    return ESCRIPTTOKEN_ARGUMENTPACK;
}
void ArgumentPackToken::LoadParams(IStream *stream) {

}
std::string ArgumentPackToken::ToString() {
    // this token is $ based on a printf in THP8
    // printf "UI WARNING: $ui_current_zone_index = %d ($ui_num_zones = %l)" d = $ui_current_zone_index l = $ui_num_zones
    
    // THDJ Wii: 4B is followed by an untokenized integer pointer into a dict
    return "$";
}