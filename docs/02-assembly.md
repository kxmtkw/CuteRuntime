
# Cute Assembly

Cute assembly is written in `.csm` files. 

### Statements

#### Instructions / Ops
Instruction seqeunces are defined by starting the statement with the name of instruction. All starting words are read as instructions and invalid instructions are rejected.
```
mov $1 $2;
```

#### Labels / Stations
Define a label with the `@` symbol.
```
@LABEL_NAME;
```
Then you can jump to it by simply referring to the name.
```
jmp LABEL_NAME;
```

### Procedures
To declare a procedure, use the `proc` keyword followed by its numerical id and the number of arguments it accepts.
```
proc ID: ARG_COUNT {

}

proc 0: 0 {

}
```