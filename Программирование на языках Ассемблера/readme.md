# Запуск
## Requirements
## Windows 10 x64
- Assembler - [MASM32](https://www.masm32.com/)
- Debugger - [OllyDbg](https://www.ollydbg.de/)
- Build - [build.bat](build.bat)
- Template

OllyDbg https://backoftut.gitbook.io/intro-cracking-with-ollydbg-2/ch-03#id-3.-flag-p-ili-flag-chyotnosti

```assembly
.386
.model flat
.stack 100h

.data
wordVar   WORD  1234h
dwordVar1 DWORD 12345678h
dwordVar2 DWORD 0ABCDEFh
byteVar   BYTE  0FFh

.code
_Start:
    mov ax,  wordVar
    mov ecx, dwordVar1
    mov edx, dwordVar2
    mov bl,  byteVar
end _Start
```
