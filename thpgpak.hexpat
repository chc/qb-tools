#pragma endian big
#pragma eval_depth 64
import std.mem;

import std.string;
import std.time;
import std.sys;

struct pak_item {
    u32 name[[format("format_pakitemttype")]];
    u32 offset;
    u32 len;
    u32 FullFileNameQBKey;
    u32 PakFullFileNameQBKey;
    u32 ShortFileNameChecksum;
    u32 fileNameQBKey;
    u32 flags;
    if(flags != 0) {
        std::print("got flags: {} {}\n",flags,$);
    }
    u8 test[len] @(offset + addressof(name));

    
};

fn format_pakitemttype(s32 type) {
 if(type == -1477114428) {
        return ".qb";
    } else if(type == -367714532) {
     return ".tvx";
 } else if(type == -1946526066) {
    return ".tex";
 }
 else if(type == 1722735159) {
    return ".mdv";
 }
 return "unknown";
};

pak_item item[2] @$;