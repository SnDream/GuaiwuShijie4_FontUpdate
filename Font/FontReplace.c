#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#if 1
/* ROM MD5: 49b984ce80ab1ebbbc215335ffa35046 */
#define BASEROM_BIN "baserom.gen"
#define OUTPUT_BIN  "output.gen"
#define FONT_ORIGIN "FontOrigin.bin"
#define FONT_UPDATE "FontUpdate.bin"
#define REP1_START  0x0017EA72
#define REP1_END    0x00185DF2
#define REP2_START  0x00175E76
#define REP2_END    0x001787B6
#else
/* ROM MD5: 02123913fd803e1e73f70bd446c4dc4f */
#define BASEROM_BIN "baserom.bin"
#define OUTPUT_BIN  "output.bin"
#define FONT_ORIGIN "FontOriginAlt.bin"
#define FONT_UPDATE "FontUpdate.bin"
#define REP1_START  0x0017E872
#define REP1_END    0x00185E86
#define REP2_START  0x00175E86
#define REP2_END    0x001786DF
#endif

#define ROM_SIZE    (2 * 1024 * 1024)
#define CHAR_NUM    (7268)
#define CHAR_SIZE   (16 * 2)

char data[ROM_SIZE] = {0};
char font_update[CHAR_NUM][CHAR_SIZE] = {0};
char font_origin[CHAR_NUM][CHAR_SIZE] = {0};

void replace(int addr)
{
    int i;
    for (i = 0; i < CHAR_NUM; i++) {
        if (memcmp(data + addr, &font_origin[i], CHAR_SIZE) == 0) {
            memcpy(data + addr, &font_update[i], CHAR_SIZE);
            return;
        }
    }
    printf("%8X is not found\n", addr);
}

void replace2(int saddr, int eaddr)
{
    int addr = saddr;
    while (addr < eaddr) {
        replace(addr);
        addr += CHAR_SIZE + 1;
    }
}

int main()
{
    int fd;
    fd = open(BASEROM_BIN, O_RDONLY);
    read(fd, data, ROM_SIZE);
    close(fd);
    fd = open(FONT_ORIGIN, O_RDONLY);
    read(fd, font_origin, CHAR_NUM * CHAR_SIZE);
    close(fd);
    fd = open(FONT_UPDATE, O_RDONLY);
    read(fd, font_update, CHAR_NUM * CHAR_SIZE);
    close(fd);

    replace2(REP1_START, REP1_END);
    replace2(REP2_START, REP2_END);
    
    fd = open(OUTPUT_BIN, O_RDWR | O_CREAT, 0777);
    write(fd, data, ROM_SIZE);
}
