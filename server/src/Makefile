HOME = ..

CC=gcc
CXX=g++
MPICXX=mpic++
LIBEXEC = ar -rc

VERSION_INFO = 	-DPROJNAME="\"server\"" \
	       				-DXONE_VERSION="\"server 1.0.0.0\"" \

INCLUDES = -I$(HOME)/include/ -I/usr/local/include/
					 
CPPFLAGS = -D_GNU_SOURCE $(INCLUDES) $(VERSION_INFO) \
					 -Wfloat-equal -Wall -Werror -Winline -pipe -O2

LDFLAGS = -L$(HOME)/src/ -lpthread -L/usr/local/lib/

HEADERS = $(wildcard *.h) 
HEADERS += $(wildcard *.hpp)
SOURCES = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SOURCES))

BIN_OUTPUT_DIR = ../bin/
LIB_OUTPUT_DIR = ../lib/
BIN_NAME = server

.PHONY: all prepare_dir echo test clean

all: copy_lib
	rm -f *.o

copy_lib: prepare_dir ${BIN_NAME}
	mv -v ${BIN_NAME} ${BIN_OUTPUT_DIR}

${BIN_NAME}: ${OBJS}
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

prepare_dir:
	if [ ! -d $(LIB_OUTPUT_DIR) ]; then rm -rf $(LIB_OUTPUT_DIR); mkdir $(LIB_OUTPUT_DIR); fi
	if [ ! -d $(BIN_OUTPUT_DIR) ]; then rm -rf $(BIN_OUTPUT_DIR); mkdir $(BIN_OUTPUT_DIR); fi

%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $< -o $@

echo:
	@echo "HEADERS = $(HEADERS)"
	@echo "SOURCES = $(SOURCES)"
	@echo "OBJS = $(OBJS)"

test:
	make -C ../test/;

clean:
	# make -C ../test/ clean;
	rm -rf $(OBJS) 
	rm -rf $(BIN_NAME)

