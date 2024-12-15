#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "zx81charcodes.h"
#include <cassert>

#define ASM_ZXIDE 1
#define ASM_SJASM 2
#define BASIC_DB 0
#define FULL_ASM 1

typedef unsigned char       BYTE;
typedef unsigned short      WORD;

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

char* zxstring(char* string) {
    size_t string_len = strlen(string);
    size_t new_len = 4 * string_len + 1;
    char* retstring = new  char[new_len]; *retstring = 0;

    size_t f;
    for (f = 0; f < string_len; f++) {
        int ascii = string[f];
        char c[4]; *c = 0;
        strcpy_s(c, 4, asciicharcodes[ascii]);
        if (0 == strlen(c)) {
            sprintf_s(c, 4, "$%02X", ascii);
        }
        strcat_s(retstring, new_len, c);
        if (f != string_len - 1) {
            strcat_s(retstring, new_len, ",");
        }
    }
    return retstring;
}
void asciiToDB(char* inputString)
{
    char* dbstring = zxstring(inputString);
    std::cout << dbstring << std::endl;
    if (dbstring) {
        delete dbstring;
    }
}
void validatePfile(char* filename)
{   
    #include <sys/stat.h>
    struct stat file_status;
    int stat_ret = stat(filename, &file_status);
    if (errno == ENOENT)
    {
        std::cout << "File does not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (file_status.st_size < 0x74 + 25 + 1) //size_of System Variable + minimal display file + 80-byte = 142 bytes MINIMUM size P file
    {
        std::cout << "File is not a ZX81 Program file";
        if (file_status.st_size == 0)
            std::cout << " (zero-length file)";
        std::cout << std::endl;
        exit(EXIT_FAILURE);
    }
}
void P2ascii(char* filename)
{
    FILE* filein;
    errno_t err = fopen_s(&filein, filename, "rb");

    unsigned char buffer[128];

    WORD PROGRAM = 16509;
    fread(buffer, 1, (PROGRAM - 16393), filein);
    BYTE StatedVersion = buffer[0];
    BYTE VERSN;//TODO = DetectVersion(inputstring);
    VERSN = StatedVersion;
    WORD DFILE = buffer[16396 - 16393] + 256 * buffer[16397 - 16393];

    char (*code2ascii)[4];
    //TODO: support Lambda 8300 character codes
    //code2ascii = zx2ascii;
    code2ascii = zx81charcodes;
    //CHRcodes = ZX81charcodes;
    /*
    if (VERSN == 0xFF) {
        code2ascii = L83toAscii;
        CHRcodes = L83charcodes;
    }
    */

    WORD z = PROGRAM;

    while (z < DFILE)
    {
        WORD lineno = 256 * fgetc(filein) + fgetc(filein);
        z = z + 2;
        char s[256];
        sprintf_s(s, "% 4u ", lineno);
        std::cout << s;
        WORD linelength = fgetc(filein) + 256 * fgetc(filein);
        z = z + 2;
        for (WORD f =0; f < linelength; f++)
        {
            BYTE d = fgetc(filein);
            z++;
            if (126 == d)
            {   //floating point marker
                fseek(filein, 5, SEEK_CUR);
                z = z + 5;
                f = f + 5;
            }
            else
            {
                std::cout << zx2ascii[d];
            }
        }
        std::cout << std::endl;
    }
    
    fclose(filein);
}
void P2asmDB(char* filename, int asm_type, int asm_file)
{
    FILE* filein;
    errno_t err = fopen_s(&filein, filename, "rb");

    unsigned char buffer[128];

    WORD PROGRAM = 16509;
    fread(buffer, 1, (PROGRAM - 16393), filein);
    BYTE StatedVersion = buffer[0];
    BYTE VERSN;// = DetectVersion(inputstring);
    VERSN = StatedVersion;
    WORD DFILE = buffer[16396 - 16393] + 256 * buffer[16397 - 16393];

    char (*code2ascii)[4];
    //TODO: support Lambda 8300 character codes
    //code2ascii = zx2ascii;
    code2ascii = zx81charcodes;
    //CHRcodes = ZX81charcodes;
    /*
    if (VERSN == 0xFF) {
        code2ascii = L83toAscii;
        CHRcodes = L83charcodes;
    }
    */
    if (asm_file == BASIC_DB)
    {
        if (DFILE > PROGRAM) {
            std::cout << "; Sjasm\n";
            std::cout << "; MACRO  be ord\n";
            std::cout << ";  db ord/256,ord mod 256\n";
            std::cout << "; ENDM\r\n\n";

            std::cout << "; ZX-IDE\n";
            std::cout << "  macro be [val] { db (val) shr 8 , (val) mod 256 }\n\n";
        }
        else {
            std::cout << "; Program contains no BASIC lines\n\n";
        }
    }
    else if (asm_file == FULL_ASM)
    {
        if (asm_type == ASM_SJASM) {
            std::cout << "; ýúÞÕ File\n";
            std::cout << "; ASSEMBLY FILE FOR SJASMPLUS v1.20.3   build command: sjasmplus demo_sj.asm --raw=demo.p\n";
            std::cout << ";                or SJASM     v0.39j    build command: sjasm demo_sj.asm demo.p\n";
            std::cout << "; GET SJASM AT https://github.com/Konamiman/Sjasm\n";
            std::cout << "; GET SJASMPLUS AT https://github.com/z00m128/sjasmplus\n\n";

            std::cout << ";defs\n";
            std::cout << ";ZX81 char codes/how to survive without ASCII\n";
            std::cout << " include zx81charcodes.inc\n";
            std::cout << ";system variables\n";
            std::cout << " include zx81sys.inc\n\n";

            std::cout << "; ===========================================================\n";
            std::cout << "; BASIC code begins\n";
            std::cout << "; ===========================================================\n\n";

            if (DFILE > PROGRAM)
            {
                std::cout << " MACRO  be ord\n";
                std::cout << "  db ord/256,ord mod 256\n";
                std::cout << " ENDM\r\n\n";
            }
        }
        else if (asm_type == ASM_ZXIDE) {
            std::cout << "; ýúÞÕ\n";
            std::cout << "; ASSEMBLY & BASIC FILE FOR ZX-IDE\n";
            std::cout << "; GET ZX-IDE 1.71.01v at https://forum.tlienhard.com/phpBB3/viewtopic.php?f=2&t=802\n";
            std::cout << "; TUTORIAL at https://www.sinclairzxworld.com/viewtopic.php?f=6&t=1064\n";
            std::cout << ";\n";
            std::cout << "; PRESS CTRL + F9 TO BUILD.P FILE\n\n";

            std::cout << "format zx81\n";
            std::cout << ";labelusenumeric\n";
            std::cout << ";LISTOFF\n\n";
            std::cout << "        // (Place this assembly file in FASMW-ZX or copy the SINCL-ZX folder here)\n";

            std::cout << "        // hardware options to be set and change defaults in ZX81DEF.INC\n";
            std::cout << "        MEMAVL     =       MEM_16K         // can be MEM_1K, MEM_2K, MEM_4K, MEM_8K, MEM_16K, MEM_32K, MEM_48K\n";
            std::cout << "                                           // default value is MEM_16K\n";
            std::cout << "        STARTMODE  EQU     SLOW_MODE       // SLOW or FAST\n";
            std::cout << "        DFILETYPE  EQU     AUTO            // COLLAPSED or EXPANDED or AUTO\n";
            std::cout << "        STARTUPMSG EQU    'CREATED WITH ZX81-IDE' // any message will be shown on screen after loading, max. 32 chars\n\n";

            std::cout << "        include 'SINCL-ZX\\ZX81.INC'        // definitions of constants\n";
            std::cout << ";LISTON\n\n";

            std::cout << "; defs - life without ASCII\n";
            std::cout << "; ZX81 char codes as Defined Bytes\n";
            std::cout << " include 'zx81charcodes.inc'\n\n";

            std::cout << "; ===========================================================\n";
            std::cout << "; BASIC code begins\n";
            std::cout << "; ===========================================================\n\n";

            if (DFILE > PROGRAM)
            {
                std::cout << " macro be [val] { db (val) shr 8 , (val) mod 256 }\r\n\n";
            }
        }

    }
    WORD z = PROGRAM;

    while (z < DFILE)
    {
        WORD lineno = 256 * fgetc(filein) + fgetc(filein);
        z = z + 2;
        WORD linelength = fgetc(filein) + 256 * fgetc(filein);
        z = z + 2;

        size_t ccs = 1 + strlen("Line0030    db 00,030                   ;LINE NUMBER\r\n"); // s is temporary string used with sprintf
        assert(ccs == 55);
        char s[55];

        char slineno[5];
        char slinelen[5];
        sprintf_s(slineno, "%04u", lineno);
        sprintf_s(slinelen, "%04u", linelength);
        sprintf_s(s, ccs, "Line%s:   be %s                     ;LINE NUMBER\n", slineno, slineno);
        std::cout << s;
        sprintf_s(s, ccs, "            dw Line%send-Line%s-3   ;LINE LENGTH\n", slineno, slineno);
        std::cout << s;

        size_t ccDB = 12 + linelength * 4 + 63; //DBs: indent + DB + 4 chars for each DB
        ccDB += 63 * linelength / 6;              //                 + variable number of FP (CrLf, indentation, comments)
        char* slineDB = new char[ccDB];
        *slineDB = 0;

        size_t ccBASIC = 156 + linelength * 8; //LINE header/footer + up to 8 chars for each keyword
        char* slineBASIC = new char[ccBASIC];
        *slineBASIC = 0;

        for (WORD f = 0; f < linelength; f++)
        {
            BYTE d = fgetc(filein);
            z++;
            if ((126 == d) && (f < linelength - 5 - 1)) //sanity check in case there's an errant 126 (e.g. embedded in machine code)
            {   //floating point marker
                size_t x = strlen(slineDB);
                if (x)
                {
                    if (slineDB[x - 1] == ',')
                        slineDB[x - 1] = 0;
                    strcat_s(slineDB, ccDB, "\n");
                }
                strcat_s(slineDB, ccDB, "            db $7E,");
                for (int f = 0; f < 4; f++)
                {
                    d = fgetc(filein);
                    sprintf_s(s, ccs, "$%02X", d);
                    strcat_s(slineDB, ccDB, s);
                    strcat_s(slineDB, ccDB, ",");
                }
                d = fgetc(filein);
                sprintf_s(s, ccs, "$%02X", d);
                strcat_s(slineDB, ccDB, s);
                strcat_s(slineDB, ccDB, "  ;floating-point number\n");
                z = z + 5;
                f = f + 5;
            }
            else if ((118 == d) && (f == linelength - 1))
            {   //end of line
                size_t x = strlen(slineDB);
                if (x)
                {
                    std::cout << "            ;" << slineBASIC << "\n";
                    if (slineDB[x - 1] == ',') {
                        slineDB[x - 1] = 0;
                        strcat_s(slineDB, ccDB, "\n");
                    }
                    std::cout << slineDB;
                    *slineDB = 0;
                }
                sprintf_s(s, ccs, "Line%send:db $76                      ;ZX_NEWLINE", slineno);
                std::cout << s << "\n";
                break;
            }
            else
            {
                size_t cc = strlen(slineDB);
                if ((cc == 0) || (slineDB[cc - 1] != ','))
                    strcat_s(slineDB, ccDB, "            db ");
                cc = strlen(zx81charcodes[d]);
                if (0 == cc)
                {
                    sprintf_s(s, ccs, "$%02X", d);
                    strcat_s(slineDB, ccDB, s);
                    strcat_s(slineBASIC, ccBASIC, zx2ascii[d]);
                }
                else
                {
                    strcat_s(slineDB, ccDB, zx81charcodes[d]);
                    strcat_s(slineBASIC, ccBASIC, zx2ascii[d]);
                }
                size_t ccs = strlen(slineDB);
                if (ccs == 1023)
                    d = d;
                strcat_s(slineDB, ccDB, ",");
            }
        }
        std::cout << std::endl;
        delete slineDB;
        delete slineBASIC;
    }
    if (asm_file == FULL_ASM)
    {
        std::cout << "; ===========================================================\n";
        std::cout << "; BASIC code ends\n";
        std::cout << "; ===========================================================\n\n";

        if (asm_type == ASM_SJASM)
        {
            std::cout << "; (ZX81 screen) - 2K-16K standard low res screen (full 32x24 display)\n";
            std::cout << "; this file can be pre-populated to have characters appear on-screen after loading\n";
            std::cout << "Display:  db $76\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 0\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 1\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 2\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 3\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,_H,_E,_L,_L,_O,__,_W,_O,_R,_L,_D,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 4\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 5\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 6\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 7\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 8\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 9\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 10\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 11\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 12\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 13\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 14\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 15\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 16\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 17\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 18\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 19\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 20\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 21\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 22\n";
            std::cout << "          db __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,$76 ; Line 23\n";
            std::cout << "DisplayEnd:\n";
            std::cout << " assert (DisplayEnd-Display) = 793\n";
            std::cout << ";close out the basic program\n";
            std::cout << "Variables:\n";
            std::cout << "VariablesEnd:   db $80\n";
            std::cout << "BasicEnd:\n\n";

            std::cout << "END";
        }
        else if (asm_type == ASM_ZXIDE)
        {
            std::cout << "        include 'SINCL-ZX\\ZX81DISP.INC'          ; include D_FILE and needed memory areas\n";
            std::cout << "VARS_ADDR:\n";
            std::cout << "        db 80h\n";
            std::cout << "WORKSPACE:\n\n";

            std::cout << "assert($ - MEMST) < MEMAVL\n";
            std::cout << "// end of program";
        }
    }
    fclose(filein);
}
int main(int argc, char* argv[])
{ 
    if (argc > 1)
        validatePfile(argv[1]);
    switch (argc) {
    case 2:
        P2ascii(argv[1]);
        exit(EXIT_SUCCESS);
    case 3:
        if (0 == strcmp(argv[2], "--a")) {
            P2asmDB(argv[1],ASM_SJASM, BASIC_DB);
            exit(EXIT_SUCCESS);
        }
        else if (0 == strcmp(argv[2], "--a:sjasm")) {
            P2asmDB(argv[1], ASM_SJASM, FULL_ASM);
            exit(EXIT_SUCCESS);
        }
        else if (0 == strcmp(argv[2], "--a:zxide")) {
            P2asmDB(argv[1], ASM_ZXIDE, FULL_ASM);
            exit(EXIT_SUCCESS);
        }
    default:
        break;
    }
    std::cout << "\nZXList7 v1.0 a P-to-BASIC lister for Sinclair ZX81 program files\n\n"; //TODO  & Lambda 8300
    std::cout << "Usage: ZXList7 file.p\n\n";

    std::cout << "For BASIC db assembly\n";
    std::cout << "Usage: ZXList7 file.p --a\n\n";

    std::cout << "For a full assembly file\n";
    std::cout << "Usage: ZXList7 file.p [--a:zxide | --a:sjasm]\n" << std::endl;
    exit(EXIT_SUCCESS);
}