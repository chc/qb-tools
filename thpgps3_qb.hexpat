#pragma endian big
#pragma eval_depth 64
import std.mem;

import std.string;
import std.time;
import std.sys;

using SStruct;
using SArray;
using find_end_of_struct;
using find_end_of_array;
using string_length;
using local_string_length;

struct AlignTo<auto Alignment> {
    padding[Alignment- ((($ - 1) % Alignment) + 1)];
} [[hidden]];

struct Offset<T> {
    u32 offset;
    if(offset > 0) {
        T data @offset;
    }
};



struct header_t {
    u32 isProcessed;
    u32 total_length;
    u32 unk1;
    u32 unk2;
    u32 unk3;
    u32 unk4;
    u32 unk5;
};

enum ESymbolType : u8 {
	ESYMBOLTYPE_NONE = 0,
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
	ESYMBOLTYPE_STRUCTUREPOINTER,
	ESYMBOLTYPE_ARRAY,
	ESYMBOLTYPE_NAME,
	ESYMBOLTYPE_INTEGER_ONE_BYTE,
	ESYMBOLTYPE_INTEGER_TWO_BYTES,
	ESYMBOLTYPE_UNSIGNED_INTEGER_ONE_BYTE,
	ESYMBOLTYPE_UNSIGNED_INTEGER_TWO_BYTES,
	ESYMBOLTYPE_ZERO_INTEGER,
	ESYMBOLTYPE_ZERO_FLOAT,
};


bitfield RootFlags {
    Unk1 : 1;
    Unk2 : 1;
    IsRoot : 1;
    Unk4 : 1;
    Unk5 : 1;
    Unk6 : 1;
    Unk7 : 1;
    ArrayChild : 1;
};

struct SInteger {
    s32 value;
};

struct SChecksumName  {
    u32 value;
};

struct SFloat {
    float value;
};

struct SString {
    char value[string_length($)];
};

struct SLocalString {
    char16 value[local_string_length($)];
};

struct SPair {
    u32 hdr;
    float x;
    float y;
};

struct SVector {
    u32 hdr;
    float x;
    float y;
    float z;
};

struct SQScript {
    u32 offset_1;
    u32 offset_2;
    u32 checksum;
    u32 uncomp_size;
    u32 comp_size;
    u8 comp_data[comp_size];
    AlignTo<4>;
};

bitfield StructFlags {
    IsStructure : 1;
    Unk2 : 1;
    IsRoot : 1;
    IsReference : 1;
    Type : 4 [[format("format_structtype")]];
};
fn format_structtype(u8 type) {
 if(type == ESymbolType::ESYMBOLTYPE_INTEGER) {
        return "ESYMBOLTYPE_INTEGER";
    } else if(type == ESymbolType::ESYMBOLTYPE_FLOAT) {
        return "ESYMBOLTYPE_FLOAT";
    } else if(type == ESymbolType::ESYMBOLTYPE_NAME) {
        return "ESYMBOLTYPE_NAME";
    } else if(type == ESymbolType::ESYMBOLTYPE_STRING) {
        return "ESYMBOLTYPE_STRING";
    }  else if(type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
        return "ESYMBOLTYPE_LOCALSTRING";
    } else if(type == ESymbolType::ESYMBOLTYPE_PAIR) {
        return "ESYMBOLTYPE_PAIR";
    } else if(type == ESymbolType::ESYMBOLTYPE_VECTOR) {
        return "ESYMBOLTYPE_VECTOR";
    } else if(type == ESymbolType::ESYMBOLTYPE_QSCRIPT) {
        return "ESYMBOLTYPE_QSCRIPT";
    } else if(type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
        return "ESYMBOLTYPE_STRUCTURE";;
    } else if(type == ESymbolType::ESYMBOLTYPE_ARRAY) {
        return "ESYMBOLTYPE_ARRAY";
    }
    return type;
       
};

struct SStructValue {
    u8 unk;
    StructFlags flags;
    u16 unk2;
    u32 name;

    if(flags.IsReference) {
        u32 data;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_INTEGER) {
        SInteger data;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_FLOAT) {
        SFloat data;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_NAME) {
        SChecksumName data;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_STRING) {
        SString *data : u32;
    }  else if(flags.Type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
        SLocalString *data : u32;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_PAIR) {
        SPair *data : u32;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_VECTOR) {
        SVector *data : u32;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_QSCRIPT) {
        SQScript data;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
        SStruct *data : u32;
    } else if(flags.Type == ESymbolType::ESYMBOLTYPE_ARRAY) {
        SArray *data : u32;
    }
       
    u32 next_offset;
    if(next_offset > 0) {   
        SStructValue next @next_offset;
    }
    
};


struct SStruct {
    u32 hdr;
    Offset<SStructValue> root;
};


struct SArray {
    u16 hdr;
    StructFlags flags;
    u8 unk_a2;
    u32 num_items;
    if(num_items > 0) {
        if(flags.Type == ESymbolType::ESYMBOLTYPE_INTEGER) {
            if(num_items == 1) {
                SInteger data[num_items];
            } else {
                u32 offset;
                SInteger data[num_items] @offset;
            }
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_FLOAT) {
            if(num_items == 1) {
                SFloat data[num_items];
            } else {
                u32 offset;
                SFloat data[num_items] @offset;
            }
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_NAME) {
            if(num_items == 1) {
                SChecksumName data[num_items];
            } else {
                u32 offset;
                SChecksumName data[num_items] @offset;
            }

        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_STRING) {
            if(num_items == 1) {
                Offset<SString> data[num_items];
            } else {
                u32 offset;
                Offset<SString> data[num_items] @offset;
            }
            AlignTo<4>;
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
            if(num_items == 1) {
                Offset<SLocalString> data[num_items];
            } else {
                u32 offset;
                Offset<SLocalString> data[num_items] @offset;
            }
            AlignTo<4>;
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_PAIR) {
            SPair data;
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_VECTOR) {
            SVector data;
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_QSCRIPT) {
            SQScript data[num_items];
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
        
            if(flags.IsReference) {
                u32 offset;
                SChecksumName data[num_items] @ offset;
            } else {
                if(num_items == 1) {
                    Offset<SStruct> data[num_items];
                } else {
                    u32 offset;
                    Offset<SStruct> data[num_items] @offset;
                }
                
            }     
        } else if(flags.Type == ESymbolType::ESYMBOLTYPE_ARRAY) {
            u32 offset;
            if(flags.IsReference) {
                Offset<SChecksumName> data[num_items] @offset;

            } else {
                Offset<SArray> data[num_items] @offset;
            }
        }
    } else {
       u32 data; //padding or otherwise null data
    }
};

struct root_symbol_t {
    u8 unk;
    RootFlags flags;
    ESymbolType type;
    u8 unk2;

    u32 name;
    u32 source_filename;

    if(type == ESymbolType::ESYMBOLTYPE_INTEGER) {
        SInteger data;
        u32 _maybe_next;
    } else if(type == ESymbolType::ESYMBOLTYPE_FLOAT) {
        SFloat data;
        u32 _maybe_next;
    } else if(type == ESymbolType::ESYMBOLTYPE_NAME) {    
        SChecksumName data;
        u32 _maybe_next;
    } else if(type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
        Offset<SLocalString> data;
        u32 _maybe_next;
        $ = std::mem::align_to(4, addressof(data.data) + (local_string_length(data.offset) * 2));
    } else if(type == ESymbolType::ESYMBOLTYPE_PAIR) {
        Offset<SPair> data;        
        $ = std::mem::align_to(4, addressof(data.data.y) + sizeof(data.data.y));
    } else if(type == ESymbolType::ESYMBOLTYPE_VECTOR) {
        Offset<SVector> data;
        $ = std::mem::align_to(4, addressof(data.data.z) + sizeof(data.data.z));
    } else if(type == ESymbolType::ESYMBOLTYPE_STRING) {
        Offset<SString> data;
        u32 _maybe_next;
        $ = std::mem::align_to(4, addressof(data.data) + (string_length(data.offset) * 2));
    } else if(type == ESymbolType::ESYMBOLTYPE_QSCRIPT) {
        SQScript data;
    } else if(type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
        SStruct *data : u32;
        $ = find_end_of_struct(addressof(data));
    } else if(type == ESymbolType::ESYMBOLTYPE_ARRAY) {
        SArray *data : u32;
        $ = find_end_of_array(addressof(data));
    }
};

fn find_end_of_struct(u32 root_value_offset) { 
    SStruct structure @root_value_offset;
    u32 offset = structure.root.offset;
    while(true) {
        SStructValue value @offset;
        if(value.next_offset == 0) {
            break;
        }
        offset = value.next_offset;
    }
    
    SStructValue value @offset;
    
    if(value.flags.IsReference) {
        return addressof(value) + sizeof(value);
    }
    if(value.flags.Type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
        offset = find_end_of_struct(addressof(value.data));
        
        u16 flags = std::mem::read_unsigned(offset, 2, std::mem::Endian::Big);
        if(flags & 0x20) {
            return offset;
        }
        SStructValue last_value @offset;

        if(last_value.flags.Type == ESymbolType::ESYMBOLTYPE_STRUCTURE) {
            return find_end_of_struct(addressof(last_value.data)) ;
        } else if(last_value.flags.Type == ESymbolType::ESYMBOLTYPE_ARRAY) {
            std::print("the array");
        } else {
            return addressof(last_value.next_offset) + sizeof(last_value.next_offset);
        }
    } else if(value.flags.Type == ESymbolType::ESYMBOLTYPE_ARRAY) {       
        return find_end_of_array(addressof(value.data));
    } else if(value.flags.Type == ESymbolType::ESYMBOLTYPE_STRING) {
        return std::mem::align_to(4, addressof(value.next_offset) + sizeof(value.next_offset) + string_length(addressof(value.data)));
    } else if(value.flags.Type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
        return std::mem::align_to(4, addressof(value.next_offset) + sizeof(value.next_offset) + local_string_length(addressof(value.data))*2);
    }
    
    return addressof(value.next_offset) + sizeof(value.next_offset);
};

fn find_end_of_array(u32 array_offset) {
    SArray arr @array_offset;
    
    if(arr.num_items == 0) {
        return addressof(arr.data) + 4;
    }
    
    if(arr.flags.Type == ESymbolType::ESYMBOLTYPE_STRUCTURE && !arr.flags.IsReference) {
        return find_end_of_struct(arr.data[arr.num_items-1].offset);
    } else if(arr.flags.Type == ESymbolType::ESYMBOLTYPE_ARRAY) {
        return find_end_of_array(arr.data[arr.num_items-1].offset);
    } else if(arr.flags.Type == ESymbolType::ESYMBOLTYPE_STRING) {
        return addressof(arr.data[arr.num_items-1].data) + string_length(addressof(arr.data[arr.num_items-1].data));
    }  else if(arr.flags.Type == ESymbolType::ESYMBOLTYPE_LOCALSTRING) {
        return addressof(arr.data[arr.num_items-1].data) + (local_string_length(addressof(arr.data[arr.num_items-1].data))*2);
    } else {
        return addressof(arr.data[arr.num_items-1]) + sizeof(arr.data[arr.num_items-1]);
    }
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


fn local_string_length(u32 offset) { //includes null char
    u32 len = 0;
    while(true) {
        u16 c = std::mem::read_unsigned(offset, 2);
        len = len + 1;
        if(c == 0) {
            break;
        }
        offset = offset + 2;
    }
    return len;
};


header_t header @ 0x00;
//root_symbol_t root[31] @$;
SStructValue sstructvalue_at_0x18C0 @ 0x18C0;