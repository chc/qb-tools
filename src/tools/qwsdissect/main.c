
#include "main.h"
WS_DLL_PUBLIC_DEF const gchar plugin_version[] = VERSION;
WS_DLL_PUBLIC_DEF const int plugin_want_major = WIRESHARK_VERSION_MAJOR;
WS_DLL_PUBLIC_DEF const int plugin_want_minor = WIRESHARK_VERSION_MINOR;

WS_DLL_PUBLIC void plugin_register(void);

int proto_qscript = -1;
gint qscript_ett_foo = -1;

static gint* qscript_ett[] = {
    &qscript_ett_foo
};

int temp_buffer_field = -1;
int unk_uint32_field = -1;
int header_len_field = -1;
int qsymbol_unk_field = -1;
int qsymbol_flags_field = -1;
int qsymbol_type_field = -1;
int qsymbol_name_field = -1;
int qsymbol_srcname_field = -1;

int qsymbol_intval_field = -1;
int qsymbol_floatval_field = -1;
int qsymbol_nextval_field = -1;
int qsymbol_offset_field = -1;

int qsymbol_script_checksum = -1;
int qsymbol_script_uncomp_sz = -1;
int qsymbol_script_comp_sz = -1;

int qsymbol_array_numitems_field = -1;

static hf_register_info standard_fields_hf[] = {
    { &qsymbol_array_numitems_field,
        { "num_items", "qscript.symbol.array.num_items",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_offset_field,
        { "offset", "qscript.symbol.qscript.offset",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_script_checksum,
        { "checksum", "qscript.symbol.qscript.checksum",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_script_uncomp_sz,
        { "uncomp_size", "qscript.symbol.qscript.uncomp_size",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_script_comp_sz,
        { "comp_size", "qscript.symbol.qscript.comp_size",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },

    { &qsymbol_floatval_field,
        { "float_value", "qscript.symbol.float_value",
        FT_FLOAT, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_intval_field,
        { "int_value", "qscript.symbol.int_value",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },

    { &qsymbol_nextval_field,
        { "next", "qscript.symbol.next",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },

    { &temp_buffer_field,
        { "temp_buffer", "qscript.temp_buffer",
        FT_BYTES, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &unk_uint32_field,
        { "unk_int32", "qscript.unk_int32",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &header_len_field,
        { "len", "qscript.header.len",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },

    //uint8 unk
    //uint8 flags
    //uint8 type
    //uint32 name
    //uint32 srcname
    //if 1 - uint32 v, uint32 next

    { &qsymbol_unk_field,
        { "unk", "qscript.qsymbol.unk",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_flags_field,
        { "flags", "qscript.qsymbol.flags",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_type_field,
        { "type", "qscript.qsymbol.type",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_name_field,
        { "name", "qscript.qsymbol.name",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &qsymbol_srcname_field,
        { "srcname", "qscript.qsymbol.srcname",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
};


void proto_register_qscript(void)
{
    proto_qscript = proto_register_protocol(
        "QScript",          /* name        */
        "qscript",          /* short name  */
        "qscript"           /* filter_name */
    );
    proto_register_field_array(proto_qscript, standard_fields_hf, array_length(standard_fields_hf));
    proto_register_subtree_array(qscript_ett, array_length(qscript_ett));
}

int dissect_qscript_symbol_integer(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_tree_add_item(tree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
    proto_tree_add_item(tree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);


    proto_tree_add_item(tree, qsymbol_intval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    return offset;
}

int dissect_qscript_symbol_float(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_tree_add_item(tree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
    proto_tree_add_item(tree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);


    proto_tree_add_item(tree, qsymbol_floatval_field, tvb, offset, sizeof(float), ENC_BIG_ENDIAN); offset += sizeof(float);
    proto_tree_add_item(tree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    return offset;
}

int dissect_qscript_symbol_script(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_tree_add_item(tree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
    proto_tree_add_item(tree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);


    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    

    proto_tree_add_item(tree, qsymbol_script_checksum, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_script_uncomp_sz, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);


    guint32 comp_sz = 0;
    proto_tree_add_item_ret_uint(tree, qsymbol_script_comp_sz, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &comp_sz); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, temp_buffer_field, tvb, offset, comp_sz, ENC_BIG_ENDIAN); offset += comp_sz;


    int align = 0;
    while((offset+align & 4)) {
        align++;
    }
    proto_tree_add_item(tree, temp_buffer_field, tvb, offset, align, ENC_BIG_ENDIAN); offset += align;

    return offset;
}
int dissect_qscript_symbol_array_struct(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_tree_add_item(tree, qsymbol_unk_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    while(true) {
        proto_item* tiv = proto_tree_add_item(tree, proto_qscript, tvb, offset, -1, ENC_NA);
        proto_tree* subtree = proto_item_add_subtree(tiv, qscript_ett_foo);
        proto_item_set_text(subtree, "Value");
        //struct item
        proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);

        guint32 type = 0;
        proto_tree_add_item_ret_uint(subtree, qsymbol_type_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN, &type); offset += sizeof(uint8_t);
        proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint16_t), ENC_BIG_ENDIAN); offset += sizeof(uint16_t);
        //
        guint32 num_array_items;
        guint32 array_type;
        guint32 next_val = 0;
        switch(type & 0xF) {
            case ESYMBOLTYPE_INTEGER:
                proto_tree_add_item(subtree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
                proto_tree_add_item(subtree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
                proto_tree_add_item_ret_uint(subtree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &next_val); offset += sizeof(uint32_t);
                break;
            case ESYMBOLTYPE_NAME:
                proto_tree_add_item(subtree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
                proto_tree_add_item(subtree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
                proto_tree_add_item_ret_uint(subtree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &next_val); offset += sizeof(uint32_t);
                break;
            case ESYMBOLTYPE_FLOAT:
                proto_tree_add_item(subtree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
                proto_tree_add_item(subtree, qsymbol_floatval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
                proto_tree_add_item_ret_uint(subtree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &next_val); offset += sizeof(uint32_t);
                break;
            case ESYMBOLTYPE_ARRAY:
                proto_tree_add_item(subtree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
                proto_tree_add_item(subtree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); //start?
                proto_tree_add_item_ret_uint(subtree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &next_val); offset += sizeof(uint32_t); //end?
                proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint16_t), ENC_BIG_ENDIAN); offset += sizeof(uint16_t);
                proto_tree_add_item_ret_uint(subtree, qsymbol_type_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN, &array_type); offset += sizeof(uint8_t);
                proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
                proto_tree_add_item_ret_uint(subtree, qsymbol_array_numitems_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &num_array_items); offset += sizeof(uint32_t);
                proto_tree_add_item(subtree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); //end?
                for(int x=0;x<num_array_items;x++) {
                    switch(array_type) {
                        case ESYMBOLTYPE_INTEGER:
                            proto_tree_add_item(subtree, qsymbol_intval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
                            break;
                        case ESYMBOLTYPE_NAME:
                            proto_tree_add_item(subtree, qsymbol_intval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
                            break;
                    }
                }
        }
        proto_item_set_end(tiv, tvb, offset);
        if(next_val == 0) {
            break;
        }
    }


    return offset;
}

int dissect_qscript_symbol_array(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_tree_add_item(tree, qsymbol_name_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t); 
    proto_tree_add_item(tree, qsymbol_srcname_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    proto_tree_add_item(tree, qsymbol_unk_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_unk_field, tvb, offset, sizeof(uint16_t), ENC_BIG_ENDIAN); offset += sizeof(uint16_t);


    guint32 type = 0;
    proto_tree_add_item_ret_uint(tree, qsymbol_type_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN, &type); offset += sizeof(uint8_t);
    proto_tree_add_item(tree, qsymbol_unk_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);

    guint32 num_items = 0;
    proto_tree_add_item_ret_uint(tree, qsymbol_array_numitems_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &num_items); offset += sizeof(uint32_t);

    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_offset_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, qsymbol_nextval_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    proto_item* tiv = proto_tree_add_item(tree, proto_qscript, tvb, offset, -1, ENC_NA);
    proto_tree* subtree_v = proto_item_add_subtree(tiv, qscript_ett_foo);
    proto_item_set_text(subtree_v, "Values");

    for(int i=0;i<num_items;i++) {
        proto_item* tiv_i = proto_tree_add_item(subtree_v, proto_qscript, tvb, offset, -1, ENC_NA);
        proto_tree* subtree_vi = proto_item_add_subtree(tiv_i, qscript_ett_foo);
        proto_item_set_text(subtree_vi, "Struct");
        offset = dissect_qscript_symbol_array_struct(tvb, pinfo, subtree_vi, data, offset);
        proto_item_set_end(tiv_i, tvb, offset);
    }

    proto_item_set_end(tiv, tvb, offset);

    
    return offset;
}

int dissect_qscript_symbol(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_item* ti = proto_tree_add_item(tree, proto_qscript, tvb, offset, -1, ENC_NA);
    proto_tree* subtree = proto_item_add_subtree(ti, qscript_ett_foo);
    proto_item_set_text(subtree, "Symbol");

    guint32 type = 0;

    proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    proto_tree_add_item(subtree, qsymbol_flags_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    proto_tree_add_item_ret_uint(subtree, qsymbol_type_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN, &type); offset += sizeof(uint8_t);
    proto_tree_add_item(subtree, qsymbol_unk_field, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);


    proto_item* tiv = proto_tree_add_item(subtree, proto_qscript, tvb, offset, -1, ENC_NA);
    proto_tree* val_tree = proto_item_add_subtree(tiv, qscript_ett_foo);
    proto_item_set_text(val_tree, "Value");
    switch(type) {
        case ESYMBOLTYPE_INTEGER:
            offset = dissect_qscript_symbol_integer(tvb, pinfo, val_tree, data, offset);
            break;
        case ESYMBOLTYPE_FLOAT:
            offset = dissect_qscript_symbol_float(tvb, pinfo, val_tree, data, offset);
            break;
        case ESYMBOLTYPE_QSCRIPT:
            offset = dissect_qscript_symbol_script(tvb, pinfo, val_tree, data, offset);
            break;
        case ESYMBOLTYPE_ARRAY:
            offset = dissect_qscript_symbol_array(tvb, pinfo, val_tree, data, offset);
        break;

    }
    proto_item_set_end(ti, tvb, offset);
    proto_item_set_end(tiv, tvb, offset);
    return offset;
    
}

/* This method dissects fully reassembled messages */
static int
    dissect_qscript(tvbuff_t* tvb, packet_info* pinfo _U_, proto_tree* tree _U_, void* data _U_)
{
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "QScript");
    int offset = 0;

    proto_item* ti = proto_tree_add_item(tree, proto_qscript, tvb, 0, -1, ENC_NA);
    proto_tree* subtree = proto_item_add_subtree(ti, qscript_ett_foo);
    proto_item_set_text(subtree, "Header");

    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, header_len_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(subtree, unk_uint32_field, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);
    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);
    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);
    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);
    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);
    offset = dissect_qscript_symbol(tvb, pinfo, tree, data, offset);

    /* TODO: implement your dissecting code */
    return tvb_captured_length(tvb);
}


static guint
    get_qscript_message_len(packet_info* pinfo _U_, tvbuff_t* tvb, int offset, void* data _U_)
{
    uint32_t message_length = tvb_get_gint32(tvb, 4, ENC_BIG_ENDIAN);
    
    return (guint)message_length;
}

static int dissect_qscript_pdu(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_)
{
    tcp_dissect_pdus(tvb, pinfo, tree, TRUE, FRAME_HEADER_LEN, get_qscript_message_len, dissect_qscript, data);
}

void proto_reg_handoff_qscript(void)
{
    static dissector_handle_t qscript_handle;

    qscript_handle = create_dissector_handle(dissect_qscript_pdu, proto_qscript);
    dissector_add_uint("tcp.port", QSCRIPT_PORT, qscript_handle);  
}

void plugin_register(void)
{
        static proto_plugin plug;

        plug.register_protoinfo = proto_register_qscript;
        plug.register_handoff = proto_reg_handoff_qscript; /* or NULL */
        proto_register_plugin(&plug);
}