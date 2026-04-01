
VR_FLAGS = -g -ggdb3

# ASSAN = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# DOP = -Wall -Wextra -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE

# VR_FLAGS += $(ASSAN)

# VR_FLAGS += $(DOP)

VR_FLAGS += -O0 -fno-inline -fno-omit-frame-pointer	

#==============================================================================================================================================

VR_COMPILER = gcc

#==============================================================================================================================================

all: task

#==============================================================================================================================================

task: OBJ obj/cJSON.o obj/main.o
	$(VR_COMPILER) obj/cJSON.o obj/main.o -o bin $(VR_FLAGS)

obj/cJSON.o: src/cJSON.c
	$(VR_COMPILER) -c -o  obj/cJSON.o src/cJSON.c $(VR_FLAGS)

obj/main.o: src/main.c
	$(VR_COMPILER) -c -o obj/main.o src/main.c $(VR_FLAGS)

OBJ:
	mkdir -p obj

clean:
	rm callgrind.out.*
	rm -rf obj
	rm bin

# valgrind --tool=callgrind ./bin dat/0030_0000.json
# kcachegrind callgrind.out.XXXXXXX