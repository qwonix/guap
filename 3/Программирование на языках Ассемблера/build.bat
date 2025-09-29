echo off
masm32\bin\ml /c /coff "template.asm"
masm32\bin\link /subsystem:console "template.obj"