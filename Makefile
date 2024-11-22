# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

CC = cl.exe
LN = link.exe
RC = rc.exe
SOURCES = .

BLDARCH = x64

PROJECT = wwhich
PPREFIX = $(SOURCES)\$(PROJECT)
WORKTOP = $(SOURCES)\build
WORKDIR = $(WORKTOP)\rel
POUTPUT = $(WORKDIR)\$(PROJECT).exe

WINVER = 0x0A00
CFLAGS = -DWIN32 -D_WIN32 -D_WIN32_WINNT=$(WINVER) -DWINVER=$(WINVER) -DWIN32_LEAN_AND_MEAN
CFLAGS = $(CFLAGS) -D_CRT_SECURE_NO_WARNINGS  -D_CRT_SECURE_NO_DEPRECATE
CFLAGS = $(CFLAGS) -DUNICODE -D_UNICODE
CLOPTS = -c -nologo -MT -W4 -O2 -Ob2 -GF -Gs0
RCOPTS = /nologo /l 0x409 /n
RFLAGS = /dNDEBUG /dWINVER=$(WINVER) /d_WIN32_WINNT=$(WINVER) /dWIN32_LEAN_AND_MEAN $(EXTRA_RFLAGS)
LFLAGS = /nologo /INCREMENTAL:NO /OPT:REF /SUBSYSTEM:CONSOLE /MACHINE:$(BLDARCH)
LFLAGS = $(LFLAGS) /NODEFAULTLIB:libucrt.lib /DEFAULTLIB:ucrt.lib $(EXTRA_LFLAGS)
LDLIBS = kernel32.lib $(EXTRA_LIBS)

!IF DEFINED(BUILD_NUMBER)
CFLAGS = $(CFLAGS) -DBUILD_NUMBER=$(BUILD_NUMBER)
RFLAGS = $(RFLAGS) /dBUILD_NUMBER=$(BUILD_NUMBER)
!ENDIF
!IF DEFINED(BUILD_VENDOR)
CFLAGS = $(CFLAGS) -DBUILD_VENDOR=$(BUILD_VENDOR)
RFLAGS = $(RFLAGS) /dBUILD_VENDOR=$(BUILD_VENDOR)
!ENDIF

OBJECTS = \
	$(WORKDIR)\$(PROJECT).obj \
	$(WORKDIR)\$(PROJECT).res

all : $(WORKDIR) $(POUTPUT)

$(WORKDIR):
	@-md $(WORKDIR) 2>NUL

{$(SOURCES)\}.c{$(WORKDIR)}.obj:
	$(CC) $(CLOPTS) $(CFLAGS) -Fo$(WORKDIR)\ $<

{$(SOURCES)\}.rc{$(WORKDIR)}.res:
	$(RC) $(RCOPTS) $(RFLAGS) -fo $@ $<

$(POUTPUT): $(OBJECTS)
	$(LN) $(LFLAGS) /out:$(POUTPUT) $(OBJECTS) $(LDLIBS)

clean:
	@-rd /S /Q $(WORKDIR) 2>NUL
