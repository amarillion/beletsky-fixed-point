#by Martijn van Iersel (amarillion@yahoo.com)

# BUILD=DEBUG
BUILD=RELEASE

LIBS = 
CFLAGS = -Iinclude -W -Wall

ALLEGRO_MODULES=allegro allegro_primitives allegro_font allegro_main allegro_dialog allegro_image allegro_audio allegro_acodec allegro_ttf allegro_color

ifeq ($(BUILD),RELEASE)
	CFLAGS += -O3 -s
	ALLEGRO_LIBS = $(addsuffix -5, $(ALLEGRO_MODULES))
endif
ifeq ($(BUILD),DEBUG)
	CFLAGS += -g -DDEBUG
	ALLEGRO_LIBS = $(addsuffix -debug-5, $(ALLEGRO_MODULES))
endif

# LIBS += `pkg-config --libs $(ALLEGRO_LIBS)`

# linux-specific stuff:
GPP = g++
BINSUF =
LFLAGS = 
# ifeq ($(BUILD),RELEASE)
# 	LIBS += `allegro-config --libs`
# endif
# ifeq ($(BUILD),DEBUG)
# 	LIBS += `allegro-config --libs debug`
# endif

OBJDIR=obj

vpath %.cpp src

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRC)))
DEP = $(patsubst %.cpp, $(OBJDIR)/%.d, $(notdir $(SRC)))
BIN = beletsky$(BINSUF)

$(shell mkdir -p $(OBJDIR) >/dev/null)

$(BIN) : $(OBJ) $(LIB)
	$(GPP) $^ -o $@ $(LIBS) $(LFLAGS)

$(OBJ) : $(OBJDIR)/%.o : %.cpp
	$(GPP) $(CFLAGS) -MMD -c $< -o $@