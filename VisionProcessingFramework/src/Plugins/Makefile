MAKE		= make --no-print-directory
MAKE_DIRS	= $(wildcard *)

default:
	@for subdir in $(MAKE_DIRS) ; do \
		(cd $$subdir && $(MAKE)) ;\
	done

clean:
	@for subdir in $(MAKE_DIRS) ; do \
		(cd $$subdir && $(MAKE) clean) ;\
	done