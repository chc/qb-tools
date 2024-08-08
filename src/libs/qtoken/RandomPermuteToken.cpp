#include "RandomPermuteToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <sstream>

RandomPermuteToken::RandomPermuteToken() {

}
RandomPermuteToken::~RandomPermuteToken() {

}
EScriptToken RandomPermuteToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE;
}
std::string RandomPermuteToken::ToString() {
    return "RandomPermute(";
}