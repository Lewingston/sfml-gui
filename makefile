
################################################################################
#                        MAKEFILE FOR SFML GUI LIBRARY                         #
################################################################################
#
# Required variables:
#
#    SFML_INCLUDE_DIR  path to the SFML include directory
#
# Required variables when building demo:
#
#    SFML_LIB_DIR      path to SFML library directory
#
# Optional variables:
#
#    SRC_DIR           source directory where the cfiles are located
#
#    OBJ_DIR           output directory for generated .o and .d files
#
#    OUT_DIR           output directory for generated .a file 
#
#    ARCHIVE_NAME      name of the generated .a file
#
#    COM               compiler
#
#    AR                archiver to generate .a file
#
#    CFLAGS            compiler flags

################################################################################
#                            DISABLE IMPLICIT RULES                            #
################################################################################

.SUFFIXES:

################################################################################
#                              PERFROM CHECKS                                  #
################################################################################

ifneq ($(MAKECMDGOALS), clean)
    ifndef SFML_INCLUDE_DIR
        $(error "SFML_INCLUDE_DIR" ist not defined!)
    endif
endif

ifeq ($(MAKECMDGOALS), demo)
    ifndef SFML_LIB_DIR
        $(error "SFML_LIB_DIR" ist no defined!)
    endif
endif

################################################################################
#                                SOURCE FILES                                  #
################################################################################

C_FILES = Widget.cpp \
          Window.cpp \
          Color.cpp \
          TextWidget.cpp \
          Font.cpp \
          Button.cpp \
          Clickable.cpp \
          Event.cpp \
          DropDownMenu.cpp \
          Selectable.cpp \
          TextInput.cpp \
          Animation.cpp \
          AnimationCollection.cpp \
          RoundedRectangleShape.cpp \
          Movable.cpp \
          CheckBox.cpp \
          Image.cpp \
          Scrollable.cpp \
          Shader.cpp

################################################################################
#                              DIRECTORY SECTION                               #
################################################################################

ifndef SRC_DIR
    SRC_DIR = ./UI/
endif

ifndef OBJ_DIR
    OBJ_DIR = ./obj/
endif

ifndef OUT_DIR
    OUT_DIR = ./bin/
endif

################################################################################
#                               LIBRARY NAME                                   #
################################################################################

ifndef ARCHIVE_NAME
    ARCHIVE_NAME = ui
endif

################################################################################
#                              GENERAL BUILD RILES                             #
################################################################################

all : $(OUT_DIR)$(ARCHIVE_NAME).a

demo : $(OUT_DIR)demo.exe

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)$(ARCHIVE_NAME).a
	rm -rf $(OUT_DIR)demo.exe

################################################################################
#                                INCLUDE FLAGS                                 #
################################################################################

INCLUDE_FLAGS = -isystem$(SFML_INCLUDE_DIR) \
                -DSFML_STATIC

################################################################################
#                                COMPILER FLAGS                                #
################################################################################

CFLAGS += -MMD

################################################################################
#                              COMPILER SECTION                                #
################################################################################

# Compiler to generate .o files from .cpp files
ifndef COM
    COM = g++
endif

# Archiver to generate .a file from .o files
ifndef AR
    AR = ar
endif

################################################################################
#                            FILE AND DIRECTORY SECTION                        #
################################################################################

# list of object files
O_FILES = $(addprefix $(OBJ_DIR),$(addsuffix .o,$(basename $(C_FILES))))

# list of dependeny files
D_FILES = $(addprefix $(OBJ_DIR),$(addsuffix .d,$(basename $(C_FILES))))

# list of output directories
OUT_DIRS = $(abspath $(addprefix $(OBJ_DIR),$(sort $(dir $(C_FILES))))) \
           $(OUT_DIR) \
           $(OBJ_DIR)

################################################################################
#                         INCLUDE DEPENDENCY FILES                             #
################################################################################

-include $(D_FILES)

################################################################################
#                              BUILD DEMO EXE                                  #
################################################################################

SFML_LINKER_FLAGS = -L$(SFML_LIB_DIR) \
                    -lsfml-network-s \
                    -lsfml-graphics-s \
                    -lsfml-window-s \
                    -lsfml-system-s \
                    -lwinmm \
                    -lopengl32 \
                    -lgdi32 \
                    -lws2_32 \
                    -lfreetype

LINKER_FLAGS = $(SFML_LINKER_FLAGS) \
               -static \
               -static-libgcc \
               -static-libstdc++

$(OUT_DIR)demo.exe : demo.cpp $(OUT_DIR)$(ARCHIVE_NAME).a | $(OUT_DIRS)
	@echo Create demo.exe
	@$(COM) -c demo.cpp -o $(OBJ_DIR)demo.o -std=c++23
	@$(COM) $(OBJ_DIR)demo.o $(OUT_DIR)$(ARCHIVE_NAME).a -o $@ $(LINKER_FLAGS)

################################################################################
#                               GENERATE FONT DATA                             #
################################################################################

$(OBJ_DIR)fontData.o : res/consolas.ttf | $(OBJ_DIR)
	@echo Generate font data
	xxd -i $< > $(OBJ_DIR)fontData.c
	$(CC) -c $(OBJ_DIR)fontData.c -o $(OBJ_DIR)fontData.o

################################################################################
#                       RULE TO GENERATE ARCHIVE FILE                          #
################################################################################

$(OUT_DIR)$(ARCHIVE_NAME).a : $(O_FILES) $(OBJ_DIR)fontData.o | $(OUT_DIR)
	@echo Create archive $@
	@$(AR) -rc $@ $(O_FILES) $(OBJ_DIR)fontData.o

################################################################################
#                  RULE TO COMPILE SOURCE FILE TO OBJECT FILE                  #
################################################################################

define generic_cpp_to_o_rule

$(OBJ_DIR)$(basename $(Item)).o : $(SRC_DIR)$(Item) | $(abspath $(OBJ_DIR)$(dir $(Item)))
	@echo Compile $(Item)
	@$(COM) $(CFLAGS) $(INCLUDE_FLAGS) -c $$< -o $$@

endef

$(foreach Item,$(C_FILES),$(eval $(generic_cpp_to_o_rule)))

################################################################################
#                      RULE TO GENERATE OUTPUT DIRECTORIES                     #
################################################################################

$(OUT_DIRS) :
	mkdir -p $@