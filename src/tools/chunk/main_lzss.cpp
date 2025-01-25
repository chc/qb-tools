#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <stdio.h>
#include <lzss.h>

int main(int argc, const char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
		return -1;
	}

	FILE* fd = fopen(argv[1], "rb");
	if (fd == NULL) {
		fprintf(stderr, "Failed to open file %s\n", argv[1]);
		return -1;
	}

	FILE* out_fd = fopen(argv[2], "wb");
	if (out_fd == NULL) {
		fprintf(stderr, "Failed to open file %s\n", argv[2]);
		return -1;
	}

	fseek(fd, 0, SEEK_END);
	long size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	char* in_buffer = new char[size];
	int len = fread(in_buffer, size, 1, fd);
	if (len != 1) {
		fprintf(stderr, "Failed to read input buffer");
		return -1;
	}
	fclose(fd);

	char* out = new char[size * 2];
	len = compress_lzss((unsigned char *)in_buffer, size, (unsigned char*)out);

 	len = fwrite(out, len, 1, out_fd);
	if (len != 1) {
		fprintf(stderr, "Failed to write output buffer");
		return -1;
	}

	fclose(fd);
	fclose(out_fd);
	delete[] out;
	delete[] in_buffer;
	return 0;
}