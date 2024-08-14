#ifndef _QSYMBOLTOKEN_H
#define _QSYMBOLTOKEN_H

#include <stdint.h>
#include <string>
#include <vector>
#include <IStream.h>

enum ESymbolType
{
    ESYMBOLTYPE_NONE=0,
    ESYMBOLTYPE_INTEGER,
    ESYMBOLTYPE_FLOAT,
    ESYMBOLTYPE_STRING,
    ESYMBOLTYPE_LOCALSTRING,
    ESYMBOLTYPE_PAIR,
    ESYMBOLTYPE_VECTOR,
    ESYMBOLTYPE_QSCRIPT,
    ESYMBOLTYPE_CFUNCTION,
    ESYMBOLTYPE_MEMBERFUNCTION,
    ESYMBOLTYPE_STRUCTURE,
	// ESYMBOLTYPE_STRUCTUREPOINTER is not really used any more. It is only supported as a valid
	// type that can be sent to AddComponent, which is an old CStruct member function that is
	// still supported for back compatibility. mType will never be ESYMBOLTYPE_STRUCTUREPOINTER
    ESYMBOLTYPE_STRUCTUREPOINTER,
    ESYMBOLTYPE_ARRAY,
    ESYMBOLTYPE_NAME,
	
	// These symbols are just used for memory optimization by the
	// CScriptStructure WriteToBuffer and ReadFromBuffer functions.
	ESYMBOLTYPE_INTEGER_ONE_BYTE,
	ESYMBOLTYPE_INTEGER_TWO_BYTES,
	ESYMBOLTYPE_UNSIGNED_INTEGER_ONE_BYTE,
	ESYMBOLTYPE_UNSIGNED_INTEGER_TWO_BYTES,
	ESYMBOLTYPE_ZERO_INTEGER,
	ESYMBOLTYPE_ZERO_FLOAT,
	
	// Warning! Don't exceed 256 entries, since Type is a uint8 in SSymbolTableEntry
	// New warning! Don't exceed 64 entries, because the top two bits of the symbol
	// type are used to indicate whether the name checksum has been compressed to
	// a 8 or 16 bit index, when WriteToBuffer writes out parameter names.

    ESYMBOLTYPE_INTERNAL_REFERENCE, //not real
};

class QSymbolToken {
    public:
        QSymbolToken();
        virtual ~QSymbolToken();
        virtual ESymbolType GetType() = 0;
		virtual void LoadParams(IStream *stream) = 0;
        virtual void LoadParamsFromArray(IStream *stream);
        uint32_t GetNextOffset();
        void SetNextOffset(uint32_t v) { m_next_offset = v; }
		static QSymbolToken *Resolve(uint8_t token);
        static void ReadSymbolsFromArray(IStream *stream, uint8_t type_flags, uint32_t num_items, QSymbolToken **output_tokens);
		virtual std::string ToString() = 0;
        void SetNameChecksum(uint32_t name) { m_name_checksum = name; }
        uint32_t GetNameChecksum() { return m_name_checksum; }
        void SetSourceChecksum(uint32_t name) { m_source_checksum = name; }
        void SetIsStructItem(bool active) { m_struct_item = active; }
        bool GetIsStructItem() { return m_struct_item; }

        virtual void Write(IStream *stream) = 0;
        virtual void WriteToArray(IStream *stream) = 0;
    protected:
        bool m_struct_item;
        uint32_t m_name_checksum;
        uint32_t m_source_checksum;
        uint32_t m_next_offset;
};


#endif //_QSYMBOLTOKEN_H