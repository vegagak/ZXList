;system variables
;Origin of a ZX81 file is always 16393

                ORG 16393
                
; System variables live here
VERSN:          DEFB 0
E_PPC:          DEFW 10
D_FILE:         DEFW Display
DF_CC:          DEFW Display+1                  ; First character of display
VARS:           DEFW Variables
DEST:           DEFW 0
E_LINE:         DEFW BasicEnd 
CH_ADD:         DEFW BasicEnd+1                 ; Simulate SAVE "X"
X_PTR:          DEFW 0
STKBOT:         DEFW BasicEnd+1
STKEND:         DEFW BasicEnd+1                 ; Empty stack
BREG:           DEFB 0
MEM:            DEFW MEMBOT
UNUSED1:        DEFB 0
DF_SZ:          DEFB 2
S_TOP:          DEFW 0                          ; Top program line number
LAST_K:         DEFW $fdbf
DEBOUN:         DEFB 0
MARGIN:         DEFB 31
NXTLIN:         DEFW 0                          ; Next line address
OLDPPC:         DEFW 0
FLAGX:          DEFB 0
STRLEN:         DEFW 0
T_ADDR:         DEFW $0c8d
SEED:           DEFW 0
FRAMES:         DEFW $f5a3
COORDS:         DEFW 0
PR_CC:          DEFB $bc
S_POSN:         DEFW $1801
CDFLAG:         DEFB $40
PRBUFF:         DEFB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,$76 ; 32 Spaces + Newline
MEMBOT:         DEFB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ; 30 zeros
UNUSED2:        DEFW 0
BASICPROG:

; End of system variables

