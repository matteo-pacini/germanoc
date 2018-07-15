# MosconiLang

Un linguaggio di programmazione ispirato e dedicato al mitico [Germano Mosconi](https://it.wikipedia.org/wiki/Germano_Mosconi).

*A programming language inspired and dedicated to [Germano Mosconi](https://en.wikipedia.org/wiki/Germano_Mosconi).*

![Germano Mosconi](germano.jpg)

# Compiler Infos

**Lexer/Parser** : mpc (parsec combinator)

**Language** : C

**Libraries** : GLib2.0 / LLVM 6.x

**Build System** : CMake

# Esempi (*Examples*)

## Hello World

    # Questo e' un commento - this is a comment.
    AMICI IN ASCOLTO, UN CORDIABLE BUONGIORNO
    METTI UN "Hello World!"
    ANDIAMO DALLA SIGLA DAI
    
## Dichiarazione variabile - *Declare a variable*

    AMICI IN ASCOLTO, UN CORDIALE BUONGIORNO
    MA COS'E' STA idiozia? 10
    METTI UNA idiozia
    ANDIAMO DALLA SIGLA DAI
    
## Operazioni su variabili - *Operations on variables*

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

# Istruzioni - *Instructions*

L'unico tipo dato manipolabile in *MosconiLang* e' un intero a 4-byte.

*MosconiLang has only got one type of data: a 4 byte integer.*

[`AMICI IN ASCOLTO, UN CORDIABLE BUONGIORNO`](https://youtu.be/GBACLpkon6Q?t=1s) Inizio programma - *Beginning of the program*

[`METTI UN/UNA <STRINGA>`](https://youtu.be/GBACLpkon6Q?t=12m14s) Stampa **STRINGA** a video - *Prints **STRINGA** to stdout*

[`METTI UN/UNA <ID>`](https://youtu.be/GBACLpkon6Q?t=12m14s) Stampa la variabile **ID** a video - *Prints content of variable **ID** to stdout*

[`MA COS'E' STO <ID>? <NUMERO>`](https://youtu.be/GBACLpkon6Q?t=1m33s) Crea una variabile di nome **ID**, avente valore **NUMERO** - *Creates a variable called **ID** with value **NUMERO***

[`DAMME CAMPO LUNGO SU <ID>`](https://youtu.be/GBACLpkon6Q?t=14m10s) Legge un intero da **stdin** nella variabile **ID** - *Reads an integer from **stdin** into variable **ID***

[`ANDIAMO DALLA SIGLA DAI`](https://youtu.be/IENTp4ZCvR0?t=2m5s) Termina il programma - *End of the program*

## Modifica variabili - *Modify variables*

Le variabili possono essere modificate una alla volte, ed e' necessario "pusharle" sullo stack
con la seguente istruzione:

*Variables can be modified one at the time, by pushing them on a stack via the following instruction:*

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

# Requisiti

## macos

    brew install cmake glib pkg-config llvm
    
## Linux (apt-get)

    sudo apt-get install build-essential \
                         llvm-dev \
                         libglib2.0-dev \
                         pkg-config \
                         cmake
    
## Windows (MINGW64)

    pacman -S mingw-w64-x86_64-toolchain \
              mingw-w64-x86_64-clang \
              mingw-w64-x86_64-llvm \
              mingw-w64-x86_64-pkg-config \ 
              mingw-w64-x86_64-glib2 \
              mingw-w64-x86_64-polly

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

## argtable3 

    Copyright (C) 1998-2001,2003-2011,2013 Stewart Heitmann
    <sheitmann@users.sourceforge.net>
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of STEWART HEITMANN nor the  names of its contributors
          may be used to endorse or promote products derived from this software
          without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    
    
    NetBSD getopt library
    =====================
    
    Copyright (c) 2000 The NetBSD Foundation, Inc.
    All rights reserved.
    
    This code is derived from software contributed to The NetBSD Foundation
    by Dieter Baron and Thomas Klausner.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. All advertising materials mentioning features or use of this software
       must display the following acknowledgement:
           This product includes software developed by the NetBSD
           Foundation, Inc. and its contributors.
    4. Neither the name of The NetBSD Foundation nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
    ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
    BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    
## mpc

    Licensed Under BSD
    
    Copyright (c) 2013, Daniel Holden
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    
    The views and conclusions contained in the software and documentation are those
    of the authors and should not be interpreted as representing official policies,
    either expressed or implied, of the FreeBSD Project.