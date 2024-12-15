# ZXList - a P-to-BASIC lister for Sinclair ZX81 program files

Source code here is C++ tailored for the command line. Includes a Visual Studio 2022 project for Windows Console (command line application)


It will generate ASCII text and send it to the console. If it looks good, you may redirect it to a file, e.g. ZXlist7 file.p > file.bas


Also includes a programmer's option to generate BASIC lines suitable for assemblers such as <a href='https://forum.tlienhard.com/phpBB3/viewtopic.php?f=2&t=802'>ZX-IDE</a> and <a href='https://github.com/Konamiman/Sjasm'>Sjasm</a> / <a href='https://github.com/z00m128/sjasmplus'>sjasmplus</a>, the commonly used cross-platform assemblers for the ZX81

# Usage

ZXList7 v1.0 a P-to-BASIC lister for Sinclair ZX81 program files

Usage: ZXList7 file.p

For BASIC db assembly
Usage: ZXList7 file.p --a

For a full assembly file
Usage: ZXList7 file.p [--a:zxide | --a:sjasm]


# Routines
There are two main routines:
* P2ascii - generates a BASIC listing in ASCII text
* P2asmDB - generates a partial assembly file in DB (Defined Byte) format. This is generated from the BASIC part of the P file

Optionally can include the full assembly file by specifying sjasm or zxide. The resulting file will assemble into a working P file

# Contributions
Contributions welcome

To do items (not yet implemented):
* add support for P files in <a href='https://www.google.com/search?q=Lambda+8300'>Lambda 8300</a>/Power 3000/Marathon 32K/Your Computer format. This is a once-popular clone of the ZX81 with sound & slightly improved graphics. The file format is arranged differently and some keyword characters are altered

# License
Use and modify Str2Zasm [as you see fit](UNLICENSE)