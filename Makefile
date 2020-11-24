# in GiB
DATASIZE=2
SOURCES = $(wildcard *.c)
GCC_BINS = $(patsubst %.c,%,$(SOURCES))
CLANG_BINS = $(patsubst %.c,%ll,$(SOURCES))

all: $(GCC_BINS) $(CLANG_BINS)

bench: bwy bwl

bwy:
	time wc -l < data/y
bwl:
	time wc -l < data/l

bml: baseline
	time ./baseline < data/l

%: %.c
	gcc -W -Wall -O4 -o $@ $<

%ll: %.c
	clang-11 -W -Wall -O3 -o $@ $<

gendata: data/y data/l

data:
	mkdir -p data

data/y: data
	yes | pv -s $(DATASIZE)g -S > $@

data/l: data
	base64 -w 127 < /dev/zero | pv -s $(DATASIZE)g -S > $@

clean:
	$(RM) $(GCC_BINS) $(CLANG_BINS)

mrproper: clean
	$(RM) -r data

.PHONY: clean mrproper gendata all bench bwy bwl bml
