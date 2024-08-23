#pragma endian big
#pragma eval_depth 64
import std.mem;

import std.string;
import std.time;
import std.sys;

struct  toc_header {
    u32 magic;
    u32 num_files;
    u32 unk;
    u32 tst[5];
};
struct file_entry {
    u32 checksum;
    u32 uncomp_size;
    u32 comp_size;
    u32 unk_1;
    u32 unk_2;
};
toc_header header @$;
file_entry files[header.num_files] @$;
std::print("item: {} {}\n", files[1280].uncomp_size, files[1280].comp_size);