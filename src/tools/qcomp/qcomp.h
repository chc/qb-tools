#ifndef _QCOMP_H
#define _QCOMP_H

#include <stdio.h>
#include <stdint.h>
#include <QScriptToken.h>
#include <vector>
#include <cassert>

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <locale>
#include <codecvt>
#include <stack>
#include <queue>

#include <FileStream.h>
#include <crc32.h>
#include "qcomp.h"

#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>
#include <EqualsToken.h>
#include <ChecksumNameToken.h>
#include <EndOfLineToken.h>
#include <EndOfLineNumberToken.h>
#include <PairToken.h>
#include <VectorToken.h>
#include <StartArrayToken.h>
#include <EndArrayToken.h>
#include <StartStructToken.h>
#include <EndStructToken.h>
#include <StringToken.h>
#include <LocalStringToken.h>
#include <ScriptToken.h>
#include <EndScriptToken.h>

#include <OpenParenthesisToken.h>
#include <CloseParenthesisToken.h>
#include <NotToken.h>

#include <AddToken.h>
#include <MinusToken.h>
#include <MultiplyToken.h>
#include <DivideToken.h>
#include <RepeatToken.h>
#include <BeginToken.h>
#include <GreaterThanEqualToken.h>
#include <GreaterThanToken.h>
#include <LessThanEqualToken.h>
#include <LessThanToken.h>
#include <DotToken.h>
#include <BreakToken.h>
#include <ReturnToken.h>
#include <AllArgsToken.h>
#include <ColonToken.h>
#include <CommaToken.h>
#include <ArgToken.h>

#include <SwitchToken.h>
#include <CaseToken.h>
#include <DefaultToken.h>
#include <EndSwitchToken.h>

#include <AndToken.h>
#include <OrToken.h>

#include <RandomRangeToken.h>

#include <RandomToken.h>
#include <RandomNoRepeatToken.h>
#include <RandomPermuteToken.h>
#include <JumpToken.h>

#if QTOKEN_SUPPORT_LEVEL > 1
    #define ENABLE_NEWIFS
    #include <FastIfToken.h>
    #include <FastElseToken.h>

    #define ENABLE_SHORTJUMP
    #include <ShortJumpToken.h>
#else
    #include <IfToken.h>
    #include <ElseToken.h>
#endif

#include <EndIfToken.h>

#if QTOKEN_SUPPORT_LEVEL > 2
    #define ENABLE_INLINEPACKSTRUCT
    #define ENABLE_ARGPACK
    #include <InlinePackStructToken.h>
    #include <ArgumentPackToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 3
    #define ENABLE_WIDESTRING
    #include <WideStringToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 4
    #define ENABLE_ELSEIF
    #define ENABLE_NOTEQUAL

    #include <ElseIfToken.h>
    #include <NotEqualToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 5
    #include <StringQSToken.h>
    #define ENABLE_SQS_TOKENS
    #include <RandomIntegerToken.h>
    #include <RandomFloatToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 6
    #include <TypeHint_Controller.h>
    #include <TypeHint_ID.h>
    #include <TypeHint_UserInput.h>
    #include <TypeHint_Callback.h>
    #include <TypeHint_Playlist.h>
#endif

class RandomData {
public:
    RandomData(RandomData* prev, RandomToken* root) {
        this->prev = prev;
        this->root = root;
        end_token = nullptr;
    }
    RandomToken* root;
    std::stack<QScriptToken*> values;
    std::stack<JumpToken*> jumps;
    QScriptToken* end_token;

    RandomData* prev;
};

enum EReadMode {
    EReadMode_ReadName,
    EReadMode_ReadPairOrVector,
    EReadMode_ReadString,
#ifdef ENABLE_SQS_TOKENS
    EReadMode_ReadSQSToken,
#endif
#ifdef ENABLE_ARGPACK
    EReadMode_ReadDollarToken,
#endif
};

//these state variables are used interchangably depending on the read mode, so the names are a bit generic due to this
typedef struct {
    int read_index;
    EReadMode read_mode;
    std::map<uint32_t, std::string> checksum_names;
    std::string current_token;
    std::string temp_token;
    int emit_type;
    int last_emitted_type;

    bool use_eol_line_numbers;
    int current_line_number;

    std::stack<QScriptToken*> if_token_list; //used for offset writing after
#ifdef ENABLE_INLINEPACKSTRUCT
    bool do_inlinestruct_token;
#endif
    bool got_negate;

#ifdef ENABLE_ARGPACK
    bool do_arg_pack;
#ifdef WITH_SYMBOL_SUPPORT
        ESymbolType argpack_type;
#endif
    bool argpack_isreqparam;
#endif

#ifdef ENABLE_WIDESTRING
    bool do_widestring;
#endif

    int case_count;
#ifdef ENABLE_SHORTJUMP
    std::stack<QScriptToken *> switch_token_list;
#endif

    std::stack<RandomData*> randoms;
    RandomData* current_random;
    bool next_is_random_start_token;
    bool skip_next_random_jump;
    bool got_empty_random;
    int random_depth;
    int mark_end_of_random;
    bool random_read_weight;
    int random_next_weight;

    int random_parenthesis_depth;
} QCompState;

extern QCompState g_QCompState;
extern FILE* mp_input_fd;

void handle_characters(std::string input, FileStream &fsout);
void handle_character(char ch, FileStream &fsout);
bool handle_keyword_check(std::string token, FileStream &fs_out);
void update_if_offsets(FileStream &fs_out);
void update_switch_offsets(FileStream& fs_out);
void update_random_offsets(FileStream& fs_out);
void emit_token(int type, FileStream &fs_out);
void emit_pair_or_vec(std::string token, FileStream &fs_out);
#ifdef ENABLE_SQS_TOKENS
void emit_sqs_token(std::string token, FileStream &fs_out);
#endif

void handle_read_dollar_token(char ch, FileStream &fs_out);
void handle_dollar_char_str(std::string &accum);
bool handle_dollar_char_accum(char ch, std::string &accum);
void append_to_random_or_delete(QScriptToken* token);
int calculate_random_token_num_items();

void emit_name(std::string name, FileStream &fs_out);
void emit_token(std::string &current_token, FileStream &fs_out);

uint32_t gen_checksum(std::string str, bool with_conversion);

#endif //_QCOMP_H