#include "qcomp.h"
#include <FileStream.h>
#include <crc32.h>
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#include <ChecksumNameToken.h>
#include <EndArrayToken.h>
#include <EndOfLineToken.h>
#include <EndStructToken.h>
#include <EqualsToken.h>
#include <FloatToken.h>
#include <IntegerToken.h>
#include <LocalStringToken.h>
#include <NameToken.h>
#include <PairToken.h>
#include <StartArrayToken.h>
#include <StartStructToken.h>
#include <StringToken.h>
#include <VectorToken.h>
#include <WideStringToken.h>

enum EReadMode {
  EReadMode_ReadName,
  EReadMode_ReadPairOrVector,
  EReadMode_ReadString,
};

// these state variables are used interchangably depending on the read mode, so
// the names are a bit generic due to this
typedef struct {
  float floats[3];
  int read_index;
  EReadMode read_mode;
  std::map<uint32_t, std::string> checksum_names;
  std::string current_token;
  int emit_type;
} QCompState;

QCompState g_QCompState;

uint32_t gen_checksum(std::string str) {
  char *name = strdup(str.c_str());
  int len = strlen(name);

  for (int i = 0; i < len; i++) {
    name[i] = tolower(name[i]);
  }

  uint32_t checksum = crc32(0, name, len);

  g_QCompState.checksum_names[checksum] = str;
  free(name);
  return checksum;
}

bool string_is_numeric(std::string &s) {
  if (s.empty()) {
    return false;
  }
  std::string::iterator it = s.begin();
  while (it != s.end()) {
    char ch = *it;
    if (!isdigit(ch) && ch != '-') {
      return false;
    }
    it++;
  }
  return true;
}
bool string_is_float(std::string &s) {
  if (s.empty()) {
    return false;
  }
  std::string::iterator it = s.begin();
  while (it != s.end()) {
    char ch = *it;
    if (!isdigit(ch) && ch != '.' && ch != '-') {
      return false;
    }
    it++;
  }
  return true;
}
void emit_token(std::string &current_token, FileStream &fs_out) {
  if (string_is_numeric(current_token)) {
    int32_t v = (int32_t)strtol(current_token.c_str(), NULL, 10);
    IntegerToken it(v);
    it.Write(&fs_out);
  } else if (string_is_float(current_token)) {
    float f = atof(current_token.c_str());
    FloatToken ft(f);
    ft.Write(&fs_out);
  } else {
    assert(!current_token.empty());
    uint32_t checksum = gen_checksum(current_token);
    NameToken nt(checksum);
    nt.Write(&fs_out);
  }
}
void emit_token(int type, FileStream &fs_out) {
  printf("token token of type: %d\n", type);
  switch (type) {
  case ESCRIPTTOKEN_EQUALS: {
    EqualsToken et;
    et.Write(&fs_out);
    break;
  }
  case ESCRIPTTOKEN_ENDOFLINE: {
    EndOfLineToken eol;
    eol.Write(&fs_out);
    break;
  }
  case ESCRIPTTOKEN_STARTARRAY: {
    StartArrayToken sat;
    sat.Write(&fs_out);
    break;
  }
  case ESCRIPTTOKEN_ENDARRAY: {
    EndArrayToken eat;
    eat.Write(&fs_out);
    break;
  }
  case ESCRIPTTOKEN_STARTSTRUCT: {
    StartStructToken sst;
    sst.Write(&fs_out);
    break;
  }
  case ESCRIPTTOKEN_ENDSTRUCT: {
    EndStructToken est;
    est.Write(&fs_out);
    break;
  }
  default:
    assert(false);
    break;
  }
  // if (type == ESCRIPTTOKEN_EQUALS) {
  //   EqualsToken et;
  //   et.Write(&fs_out);
  // } else if (type == ESCRIPTTOKEN_ENDOFLINE) {
  //   EndOfLineToken eol;
  //   eol.Write(&fs_out);
  // } else if (type == ESCRIPTTOKEN_STARTARRAY) {
  //   StartArrayToken sat;
  //   sat.Write(&fs_out);
  // } else if (type == ESCRIPTTOKEN_ENDARRAY) {
  //   EndArrayToken eat;
  //   eat.Write(&fs_out);
  // } else if (type == ESCRIPTTOKEN_STARTSTRUCT) {
  //   StartStructToken sst;
  //   sst.Write(&fs_out);
  // } else if (type == ESCRIPTTOKEN_ENDSTRUCT) {
  //   EndStructToken est;
  //   est.Write(&fs_out);
  // } else {
  //   assert(false);
  // }
}

void handle_read_name(char ch, FileStream &fs_out) {
  switch (ch) {
  case '=':
    g_QCompState.emit_type = ESCRIPTTOKEN_EQUALS;
    break;
  case '(':
    g_QCompState.read_mode = EReadMode_ReadPairOrVector;
    return;
    break;
  case '[':
    g_QCompState.emit_type = ESCRIPTTOKEN_STARTARRAY;
    break;
  case ']':
    g_QCompState.emit_type = ESCRIPTTOKEN_ENDARRAY;
    break;
  case '{':
    g_QCompState.emit_type = ESCRIPTTOKEN_STARTSTRUCT;
    break;
  case '}':
    g_QCompState.emit_type = ESCRIPTTOKEN_ENDSTRUCT;
    break;
  case ' ':
  case '\t':
  case '\r':
    return;
  case '\n':
    g_QCompState.emit_type = ESCRIPTTOKEN_ENDOFLINE;
    break;
  case '\'':
    g_QCompState.read_mode = EReadMode_ReadString;
    g_QCompState.emit_type = ESCRIPTTOKEN_LOCALSTRING;
    return;
    break;
  case '"':
    g_QCompState.read_mode = EReadMode_ReadString;
    g_QCompState.emit_type = ESCRIPTTOKEN_STRING;
    return;
    break;
  }
  if (g_QCompState.emit_type != 0) {
    if (!g_QCompState.current_token.empty()) {
      emit_token(g_QCompState.current_token, fs_out);
      g_QCompState.current_token.clear();
    }

    emit_token(g_QCompState.emit_type, fs_out);
    g_QCompState.emit_type = 0;
    return;
  }
  g_QCompState.current_token += ch;
}
void emit_pair_or_vec(FileStream &fs_out) {
  if (g_QCompState.read_index == 1) { // pair
    PairToken pt(g_QCompState.floats[0], g_QCompState.floats[1]);
    pt.Write(&fs_out);
  } else if (g_QCompState.read_index == 2) {
    VectorToken vt(g_QCompState.floats[0], g_QCompState.floats[1],
                   g_QCompState.floats[2]);
    vt.Write(&fs_out);
  } else {
    assert(false);
  }
}
void handle_read_pair_or_vec(char ch, FileStream &fs_out) {
  switch (ch) {
  case ')':
    g_QCompState.floats[g_QCompState.read_index] =
        atof(g_QCompState.current_token.c_str());
    emit_pair_or_vec(fs_out);
    g_QCompState.current_token.clear();
    g_QCompState.read_index = 0;
    g_QCompState.read_mode = EReadMode_ReadName;
    break;
  case ',':
    g_QCompState.floats[g_QCompState.read_index++] =
        atof(g_QCompState.current_token.c_str());
    g_QCompState.current_token.clear();
    break;
  default:
    g_QCompState.current_token += ch;
    break;
  }
}
void handle_read_string(char ch, FileStream &fs_out) {
  bool end_read = false;
  if (ch == '"' && g_QCompState.emit_type == ESCRIPTTOKEN_STRING) {
    end_read = true;
  } else if (ch == '\'' && g_QCompState.emit_type == ESCRIPTTOKEN_LOCALSTRING) {
    end_read = true;
  } else {
    g_QCompState.current_token += ch;
  }

  if (end_read) {
    printf("str read: %s\n", g_QCompState.current_token.c_str());
    if (g_QCompState.emit_type == ESCRIPTTOKEN_STRING) {
      StringToken st(g_QCompState.current_token);
      st.Write(&fs_out);
    } else if (g_QCompState.emit_type == ESCRIPTTOKEN_LOCALSTRING) {
      LocalStringToken lst(g_QCompState.current_token);
      lst.Write(&fs_out);
    } else {
      assert(false);
    }
    g_QCompState.read_mode = EReadMode_ReadName;
    g_QCompState.emit_type = 0;
    g_QCompState.current_token.clear();
  }
}
int main(int argc, const char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
    return -1;
  }

  FILE *mp_input_fd = fopen(argv[1], "r");

  if (!mp_input_fd) {
    fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
    return -1;
  }

  FileStream fsout(argv[2], true);

  if (!fsout.IsFileOpened()) {
    fprintf(stderr, "Failed to open file: %s\n", argv[2]);
    return -1;
  }

  g_QCompState.read_mode = EReadMode_ReadName;

  while (true) {
    int ch = fgetc(mp_input_fd);
    if (ch == EOF) {
      break;
    }
    if (g_QCompState.read_mode == EReadMode_ReadName) {
      handle_read_name(ch, fsout);
    } else if (g_QCompState.read_mode == EReadMode_ReadPairOrVector) {
      handle_read_pair_or_vec(ch, fsout);
    } else if (g_QCompState.read_mode == EReadMode_ReadString) {
      handle_read_string(ch, fsout);
    }
  }

  if (!g_QCompState.current_token.empty()) {
    printf("doing final emit\n");
    emit_token(g_QCompState.current_token, fsout);
  }

  std::map<uint32_t, std::string>::iterator it =
      g_QCompState.checksum_names.begin();
  while (it != g_QCompState.checksum_names.end()) {
    std::pair<uint32_t, std::string> p = *it;
    it++;
    if (p.first == 0 || p.second.empty())
      continue;
    ChecksumNameToken token(p.first, p.second.c_str());
    token.Write(&fsout);
  }
  fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);
  return 0;
}
