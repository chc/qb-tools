#ifndef _QSCRTIPTTOKEN_H
#define _QSCRTIPTTOKEN_H

#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include "IStream.h"

enum EScriptToken
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
#if QTOKEN_SUPPORT_LEVEL > 1
	ESCRIPTTOKEN_KEYWORD_USEHEAP, // 69
	ESCRIPTTOKEN_KEYWORD_UNKNOWN, // 70
	ESCRIPTTOKEN_KEYWORD_FASTIF, // 71
	ESCRIPTTOKEN_KEYWORD_FASTELSE, //72
	ESCRIPTTOKEN_SHORTJUMP, // 73
#if QTOKEN_SUPPORT_LEVEL > 2
	ESCRIPTTOKEN_INLINEPACKSTRUCT, // 74
	ESCRIPTTOKEN_ARGUMENTPACK, // 75
#if QTOKEN_SUPPORT_LEVEL > 3
	ESCRIPTTOKEN_WIDESTRING, // 76
#if QTOKEN_SUPPORT_LEVEL > 4
	ESCRIPTTOKEN_NOTEQUAL, // 77
#if QTOKEN_SUPPORT_LEVEL > 5
	ESCRIPTTOKEN_STRINGQS, // 78
	ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT, // 79
	ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER, // 80
#endif // QTOKEN_SUPPORT_LEVEL > 2
#endif // QTOKEN_SUPPORT_LEVEL > 3
#endif // QTOKEN_SUPPORT_LEVEL > 4
#endif // QTOKEN_SUPPORT_LEVEL > 5
	
#endif // QTOKEN_SUPPORT_LEVEL > 1
	// Warning! Do not exceed 256 entries, since these are stored in bytes.
};

typedef struct {
    uint16_t weight;
    uint32_t offset;
} RandomOffset;

typedef struct {
	std::string token;
	size_t offset;
	bool use_next_jump_offset;
} TokenInjection;

class QScriptToken {
    public:
        QScriptToken();
        virtual ~QScriptToken();
        virtual EScriptToken GetType() = 0;
		virtual void LoadParams(IStream *stream) = 0;
		virtual void Write(IStream *stream);
		static QScriptToken *Resolve(uint8_t token);
		virtual std::string ToString() = 0;
		virtual int	GetPreTabOffset();
		virtual int GetPostTabOffset();
		virtual std::vector<TokenInjection> GetInjections();
		size_t GetFileOffset() { return m_file_offset; }
		void SetFileOffset(size_t offset) { m_file_offset = offset; }
        bool AppendSpaceToString = false;
	protected:
        size_t m_file_offset;
};
#endif //_QSCRTIPTTOKEN_H
