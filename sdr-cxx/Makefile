CC := gcc
CXX := g++

MKDIR := mkdir -p
RM := rm
CP := cp
CHMOD := chmod 755

SRCDIR := ./src

makeObjs = $(patsubst %.cpp,%.o,$(shell find $(SRCDIR) -name *.cpp))
compilerInfo = $(shell pkg-config --$1 $2)
libInfo = $(call compilerInfo,libs,$1)
hdrInfo = $(call compilerInfo,cflags,$1)

LIBS := SoapySDR kfr sndfile portaudio
HDR_INFO := $(call hdrInfo,$(LIBS))
LIB_INFO := $(call libInfo,$(LIBS))

CFILES := $(shell ls -1 $(SRCDIR)/*.cpp)



CPPFLAGS := -Wall -g -O3 -pthread -I$(SRCDIR) -I.
CXXFLAGS := -std=c++17 -c -fmessage-length=0 -fPIC $(HDR_INFO)
LDFLAGS := $(LIB_INFO)


APP := test
OBJ := $(call makeObjs,$(SRCDIR))

run: $(APP)
	./$(APP) 1.518

.PHONY: app
app: 	$(APP)

$(APP):	$(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@
	$(CHMOD) $@
	
clean:
	$(RM) $(OBJ)
	
.PHONY: list
list: 
	$(info Source is $(CFILES))
	$(info Source dir is $(SRCDIR))
	$(info File list is $(OBJ))
	$(info Compiler flags are $(CPPFLAGS) $(CXXFLAGS))
	$(info Linker flags are $(LDFLAGS))

.PHONY: gcc	
gcc:
	$(shell $(CC) -v) 
	
%.o: %.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $@ $<