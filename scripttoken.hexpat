//#pragma endian big
#pragma eval_depth 64
import std.mem;

import std.string;
import std.time;
import std.sys;


struct AlignTo<auto Alignment> {
    padding[Alignment- ((($ - 1) % Alignment) + 1)];
} [[hidden]];


struct AlignToPos<auto Alignment, auto Position> {
    padding[Alignment- (((Position - 1) % Alignment) + 1)];
} [[hidden]];

 using ScriptToken;

enum EScriptToken : u8
{
	// Misc
	ESCRIPTTOKEN_ENDOFFILE,			// 0
	ESCRIPTTOKEN_ENDOFLINE,			// 1
	ESCRIPTTOKEN_ENDOFLINENUMBER,   // 2
	ESCRIPTTOKEN_STARTSTRUCT,       // 3
	ESCRIPTTOKEN_ENDSTRUCT,         // 4
	ESCRIPTTOKEN_STARTARRAY,        // 5
	ESCRIPTTOKEN_ENDARRAY,          // 6
	ESCRIPTTOKEN_EQUALS,            // 7
	ESCRIPTTOKEN_DOT,               // 8
	ESCRIPTTOKEN_COMMA,             // 9
	ESCRIPTTOKEN_MINUS,             // 10
	ESCRIPTTOKEN_ADD,               // 11
	ESCRIPTTOKEN_DIVIDE,            // 12
	ESCRIPTTOKEN_MULTIPLY,          // 13
	ESCRIPTTOKEN_OPENPARENTH,       // 14
	ESCRIPTTOKEN_CLOSEPARENTH,      // 15

	// This is ignored by the interpreter.
	// Allows inclusion of source level debugging info, eg line number.
	ESCRIPTTOKEN_DEBUGINFO,			// 16

	// Comparisons
	ESCRIPTTOKEN_SAMEAS,			// 17
	ESCRIPTTOKEN_LESSTHAN,			// 18
	ESCRIPTTOKEN_LESSTHANEQUAL,     // 19
	ESCRIPTTOKEN_GREATERTHAN,       // 20
	ESCRIPTTOKEN_GREATERTHANEQUAL,  // 21

	// Types
	ESCRIPTTOKEN_NAME,				// 22
	ESCRIPTTOKEN_INTEGER,			// 23
	ESCRIPTTOKEN_HEXINTEGER,        // 24
    ESCRIPTTOKEN_ENUM,              // 25
	ESCRIPTTOKEN_FLOAT,             // 26
	ESCRIPTTOKEN_STRING,            // 27
	ESCRIPTTOKEN_LOCALSTRING,       // 28
	ESCRIPTTOKEN_ARRAY,             // 29
	ESCRIPTTOKEN_VECTOR,            // 30
	ESCRIPTTOKEN_PAIR,				// 31

	// Key words
	ESCRIPTTOKEN_KEYWORD_BEGIN,		// 32
	ESCRIPTTOKEN_KEYWORD_REPEAT,    // 33
	ESCRIPTTOKEN_KEYWORD_BREAK,     // 34
	ESCRIPTTOKEN_KEYWORD_SCRIPT,    // 35
	ESCRIPTTOKEN_KEYWORD_ENDSCRIPT, // 36
	ESCRIPTTOKEN_KEYWORD_IF,        // 37
	ESCRIPTTOKEN_KEYWORD_ELSE,      // 38
	ESCRIPTTOKEN_KEYWORD_ELSEIF,    // 39
	ESCRIPTTOKEN_KEYWORD_ENDIF,		// 40
	ESCRIPTTOKEN_KEYWORD_RETURN,	// 41
    
    ESCRIPTTOKEN_UNDEFINED,			// 42
	
	// For debugging					  
	ESCRIPTTOKEN_CHECKSUM_NAME,		// 43
	
	// Token for the <...> symbol					
	ESCRIPTTOKEN_KEYWORD_ALLARGS,	// 44
	// Token that preceds a name when the name is enclosed in < > in the source.
	ESCRIPTTOKEN_ARG,				// 45
	
	// A relative jump. Used to speed up if-else-endif and break statements, and
	// used to jump to the end of lists of items in the random operator.
	ESCRIPTTOKEN_JUMP,				// 46
	// Precedes a list of items that are to be randomly chosen from.
	ESCRIPTTOKEN_KEYWORD_RANDOM,    // 47

	// Precedes two integers enclosed in parentheses.
	ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE,	// 48
	
	// Only used internally by qcomp, never appears in a .qb
	ESCRIPTTOKEN_AT,				// 49
	
	// Logical operators
	ESCRIPTTOKEN_OR,				// 50
	ESCRIPTTOKEN_AND,				// 51
	ESCRIPTTOKEN_XOR,				// 52
									
	// Shift operators
	ESCRIPTTOKEN_SHIFT_LEFT,		// 53
	ESCRIPTTOKEN_SHIFT_RIGHT,		// 54
	
	// These versions use the Rnd2 function, for use in certain things so as not to mess up
	// the determinism of the regular Rnd function in replays.
	ESCRIPTTOKEN_KEYWORD_RANDOM2,		// 55
	ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE2, // 56
	
	ESCRIPTTOKEN_KEYWORD_NOT,			// 57
	ESCRIPTTOKEN_KEYWORD_AND,			// 58
	ESCRIPTTOKEN_KEYWORD_OR,            // 59
	ESCRIPTTOKEN_KEYWORD_SWITCH,       	// 60
	ESCRIPTTOKEN_KEYWORD_ENDSWITCH,   	// 61
	ESCRIPTTOKEN_KEYWORD_CASE,          // 62
	ESCRIPTTOKEN_KEYWORD_DEFAULT,		// 63

	ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT,	// 64
	ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE,	// 65

	ESCRIPTTOKEN_COLON,		// 66
	
	// These are calculated at runtime in the game code by PreProcessScripts,
	// so they never appear in a qb file.
	ESCRIPTTOKEN_RUNTIME_CFUNCTION,	// 67
	ESCRIPTTOKEN_RUNTIME_MEMBERFUNCTION, // 68
	
	ESCRIPTTOKEN_KEYWORD_USEHEAP,
	ESCRIPTTOKEN_KEYWORD_UNKNOWN,
	ESCRIPTTOKEN_KEYWORD_FASTIF,
	ESCRIPTTOKEN_KEYWORD_FASTELSE,
	ESCRIPTTOKEN_SHORTJUMP,
	ESCRIPTTOKEN_INLINEPACKSTRUCT,
	ESCRIPTTOKEN_ARGUMENTPACK,
	ESCRIPTTOKEN_WIDESTRING,
	ESCRIPTTOKEN_NOTEQUAL,
	ESCRIPTTOKEN_STRINGQS,
	ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT,
	ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER,
	
	// Warning! Do not exceed 256 entries, since these are stored in bytes.
};

fn string_length(u32 offset) { //includes null char
    u32 len = 0;
    while(true) {
        u8 c = std::mem::read_unsigned(offset, 1);
        len = len + 1;
        if(c == 0) {
            break;
        }
        offset = offset + 1;
    }
    return len;
};

fn is_single_byte_token(EScriptToken token) {
 if(token == EScriptToken::ESCRIPTTOKEN_EQUALS) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_LESSTHAN) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ARG) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ADD) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_MINUS) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_MULTIPLY) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_DIVIDE) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_DOT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_SCRIPT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_ENDSCRIPT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_STARTSTRUCT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ENDSTRUCT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_RETURN) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_STARTARRAY) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ENDARRAY) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ENDOFFILE) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ENDOFLINE) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_COLON) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_OPENPARENTH) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_CLOSEPARENTH) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ARGUMENTPACK) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_ENDIF) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_BEGIN) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_REPEAT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_NOT) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_GREATERTHAN) {
    } else {    
        return 0;
    }
    return 1;
};

struct RandomOffset {
    u32 offset;
    ScriptToken randomJump @ (addressof(offset)+ sizeof(offset)+ offset);
};

u32 last_start_script;
struct ScriptToken {
    EScriptToken token;
    
    if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_SCRIPT) {
        last_start_script = $+5;
    }
    
    if(is_single_byte_token(token)) {
    } else if(token == EScriptToken::ESCRIPTTOKEN_ENDOFLINENUMBER) {
        u32 number;
    } else if(token == EScriptToken::ESCRIPTTOKEN_NAME) {
        u32 name;
    } else if(token == EScriptToken::ESCRIPTTOKEN_CHECKSUM_NAME) {
        u32 name;
        char name_str[string_length($)];
    } else if(token == EScriptToken::ESCRIPTTOKEN_INTEGER) {
        s32 value;
    } else if(token == EScriptToken::ESCRIPTTOKEN_FLOAT) {
        float value;
    } else if(token == EScriptToken::ESCRIPTTOKEN_PAIR) {
        float x;
        float y;
    }  else if(token == EScriptToken::ESCRIPTTOKEN_VECTOR) {
        float x;
        float y;
        float z;
    } else if(token == EScriptToken::ESCRIPTTOKEN_STRING) {
        u32 len;
        char data[len];
    }  else if(token == EScriptToken::ESCRIPTTOKEN_LOCALSTRING) {
        u32 len;
        char16 data[len/2];

    }   else if(token == EScriptToken::ESCRIPTTOKEN_WIDESTRING) {
        u32 len;
        char16 data[len/2];

    }  else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_FASTIF) {
        u16 offset;        
        //ScriptToken jump_token @(addressof(offset) + offset);
    }  else if(token == EScriptToken::ESCRIPTTOKEN_JUMP) {
        u32 offset;
        ScriptToken jump_token @(addressof(offset) + sizeof(offset) + offset);
    }   else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_RANDOM) {
        u32 num_items;
        u16 weights[num_items];
        //u32 offsets[num_items];
        RandomOffset offsets[num_items];
    } else if(token == EScriptToken::ESCRIPTTOKEN_INLINEPACKSTRUCT) {
        u16 len;
        u32 offset = $ - last_start_script;
        AlignToPos<4, offset>;
        //std::print("offset: {}", offset );
        char data[len];
    } else if(token == EScriptToken::ESCRIPTTOKEN_KEYWORD_FASTELSE) {
        u16 offset;
        //ScriptToken jump_token @(addressof(offset) + offset);
    }
     else {
        std::error(std::format("Unhandled Token: {}", token));
    }
};
ScriptToken t[while(!std::mem::eof())] @0;