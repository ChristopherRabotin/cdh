# The following is mostly a copy from the root Makefile.test .

all qemu pristine clean:
	$(MAKE) -C cdh $@

run : qemu
