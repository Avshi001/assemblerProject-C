program: main.o bitEncoding.o firstPass.o lineParsing.o preAssembler.o secondPass.o structExternal.o structLabel.o structMacro.o utils.o
	gcc -g -ansi -Wall -pedantic main.o bitEncoding.o firstPass.o lineParsing.o preAssembler.o secondPass.o structExternal.o structLabel.o structMacro.o utils.o -o program

main.o: main.c programData.h assemblerStructData.h programPrototypes.h preAssemblerPrototypes.h assemblerPrototypes.h
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o

bitEncoding.o: bitEncoding.c programPrototypes.h programExternalVars.h
	gcc -c -g -ansi -Wall -pedantic bitEncoding.c -o bitEncoding.o

firstPass.o: firstPass.c programExternalVars.h assemblerPrototypes.h assemblerStructData.h programData.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic firstPass.c -o firstPass.o

lineParsing.o: lineParsing.c programPrototypes.h programData.h
	gcc -c -g -ansi -Wall -pedantic lineParsing.c -o lineParsing.o

preAssembler.o: preAssembler.c programExternalVars.h preAssemblerPrototypes.h preAssemblerStructData.h programData.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic preAssembler.c -o preAssembler.o

secondPass.o: secondPass.c programExternalVars.h assemblerPrototypes.h assemblerStructData.h programData.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic secondPass.c -o secondPass.o

structExternal.o: structExternal.c programData.h assemblerStructData.h
	gcc -c -g -ansi -Wall -pedantic structExternal.c -o structExternal.o

structLabel.o: structLabel.c programExternalVars.h assemblerStructData.h programData.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic structLabel.c -o structLabel.o

structMacro.o: structMacro.c preAssemblerStructData.h programData.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic structMacro.c -o structMacro.o

utils.o: utils.c programData.h programExternalVars.h programPrototypes.h
	gcc -c -g -ansi -Wall -pedantic utils.c -o utils.o
