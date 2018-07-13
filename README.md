# MosconiLang

Un linguaggio di programmazione ispirato e dedicato al mitico Germano Mosconi.

![Germano Mosconi](germano.jpg)

# Compiler Infos

**Lexer/Parser** : mpc (parsec combinator)
**Language** : C
**Libraries** : GLib/LLVM
**Build System** : CMake

*MosconiLang* viene tradotto in assembly poi compilato per il sistema che invoca il compilatore.

# Esempi

## Hello World

    # Questo e' un commento.
    AMICI IN ASCOLTO, UN CORDIABLE BUONGIORNO
    METTI UN "Hello World!"
    ANDIAMO DALLA SIGLA DAI
    
## Dichiarazione variabile

    AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO
    MA COS'E' STA idiozia? 10
    METTI UNA idiozia
    ANDIAMO DALLA SIGLA DAI
    
## Operazioni su variabili

    AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO

    MA COS'E' STO donadoni? 0 
    MA COS'E' STA porta? 0 

    DAMME CAMPO LUNGO SU donadoni
    DAMME CAMPO LUNGO SU porta

    ANDIAMO ALLE NOTIZIE SU donadoni
    DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON porta # donadoni = donadoni + porta
    DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON 5 # donadoni = donadoni + 5
    L'ATLETA DI FONDO E' STATO PRECEDUTO DA 5 # donadoni = donadoni - 5
    ME TOCCA RIFARE TUTTO DA CAPO

    METTI UN donadoni

    ANDIAMO DALLA SIGLA DAI

# Istruzioni

L'unico tipo dato manipolabile in *MosconiLang* e' un intero a 4-byte.

[`AMICI IN ASCOLTO, UN CORDIABLE BUONGIORNO`](https://youtu.be/GBACLpkon6Q?t=1s) Inizio programma.

[`METTI UN/UNA <STRINGA>`](https://youtu.be/GBACLpkon6Q?t=12m14s) Stampa **STRINGA** a video.

[`METTI UN/UNA <ID>`](https://youtu.be/GBACLpkon6Q?t=12m14s) Stampa la variabile **ID** a video.

[`MA COS'E' STO <ID>? <NUMERO>`](https://youtu.be/GBACLpkon6Q?t=1m33s) Crea una variabile di nome **ID**, avente valore **NUMERO**.

[`DAMME CAMPO LUNGO SU <ID>`](https://youtu.be/GBACLpkon6Q?t=14m10s) Legge un intero da **stdin** nella variabile **ID**.

[`ANDIAMO DALLA SIGLA DAI`](https://youtu.be/IENTp4ZCvR0?t=2m5s) Termina il programma.

## Modifica variabili

Le variabili possono essere modificate una alla volte, ed e' necessario "pusharle" sullo stack
con la seguente istruzione:

`ANDIAMO ALLE NOTIZIE SU/SULLA <ID>`

Le seguenti operazioni sono ora disponibili (il valore verra' scritto nella variabile "pushata" sullo stack):

**x += ID**

`DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON <ID>`

**x += INT**

`DOMANI A ZURIGO SI SVOLGERANNO GLI ACCOPPIAMENTI CON <INT>`

**x -= ID**

`L'ATLETA DI FONDO E' STATO PRECEDUTO DA <ID>`

**x -= INT**

`L'ATLETA DI FONDO E' STATO PRECEDUTO DA <INT>`

Per terminare la serie di operazioni, si utilizzi la seguente istruzione:

`ME TOCCA RIFARE TUTTO DA CAPO` 

# Compilazione

# Licenses

## MosconiC / MosconiLang

    MIT License

    Copyright (c) 2017-2018 Matteo Pacini

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
