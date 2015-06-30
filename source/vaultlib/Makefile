SRC_DIR = ./
OBJ_DIR = .objs
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)

CC = gcc
CXX = g++
AR = ar
LD = g++

INC = -I. -I../lib
CFLAGS = -pedantic-errors -pedantic -Wfatal-errors -Wextra -Wall -std=gnu++1y
CFLAGSEXT = 

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG
CFLAGSEXT_RELEASE = $(CFLAGSEXT)
OBJDIR_RELEASE = $(OBJ_DIR)/Release
DEP_RELEASE =
OUT_RELEASE = $(SRC_DIR)/libvaultlib.a
OBJ_RELEASE = $(addprefix $(OBJDIR_RELEASE)/,$(notdir $(CPP_FILES:.cpp=.o)))

INC_DEBUG = $(INC) -I../lib/stack_trace/include
CFLAGS_DEBUG = $(CFLAGS) -gstabs -DVAULTMP_DEBUG -DPACKAGE -DPACKAGE_VERSION
CFLAGSEXT_DEBUG = $(CFLAGSEXT)
OBJDIR_DEBUG = $(OBJ_DIR)/Debug
DEP_DEBUG =
OUT_DEBUG = $(SRC_DIR)/libvaultlibd.a
OBJ_DEBUG = $(addprefix $(OBJDIR_DEBUG)/,$(notdir $(CPP_FILES:.cpp=.o)))

RD = rm -rf
DEL = $(RD)
MKDIR = mkdir -p
FixPath= $1
COPY = cp

ifneq ($(shell uname -s), Linux)
	ifneq ($(shell uname -o), Cygwin)
		ifneq ($(shell uname -o), Msys)
			FixPath = $(subst /,\,$1)
			RD = rd /s/q
			DEL = del /q
			MKDIR = md
			COPY = copy
			CFLAGSEXT_RELEASE = $(CFLAGSEXT) -DWIN32
		endif
	endif
endif

all: release debug

clean: clean_release clean_debug

release: before_release out_release

debug: before_debug out_debug

before_release:
ifneq ($(wildcard $(OBJDIR_RELEASE)/.*),)
	echo "Found " $(OBJDIR_RELEASE)
else
	$(MKDIR) $(call FixPath,$(OBJDIR_RELEASE))
endif

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(AR) rv $(OUT_RELEASE) $(OBJ_RELEASE)

$(OBJDIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS_RELEASE) $(CFLAGSEXT_RELEASE) $(INC_RELEASE) -c $< -o $@

clean_release:
ifneq ($(wildcard $(call FixPath,$(OUT_RELEASE))),)
	$(DEL) $(call FixPath,$(OUT_RELEASE)) 
endif

before_debug:
ifneq ($(wildcard $(OBJDIR_DEBUG)/.*),)
	echo "Found " $(OBJDIR_DEBUG)
else
	$(MKDIR) $(call FixPath,$(OBJDIR_DEBUG))
endif

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(AR) -rv $(OUT_DEBUG) $(OBJ_DEBUG)

$(OBJDIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS_DEBUG) $(CFLAGSEXT_DEBUG) $(INC_DEBUG) -c $< -o $@

clean_debug:
ifneq ($(wildcard $(call FixPath,$(OUT_DEBUG))),)
	$(DEL) $(call FixPath,$(OUT_DEBUG)) 
endif

.PHONY: before_release clean_release before_debug clean_debug out_debug