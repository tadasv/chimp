ACLOCAL_AMFLAGS = -I config

SUBDIRS = src

DIST_SUBDIRS = src

EXTRA_DIST = \
	autogen.sh	\
	README.md	\
	LICENSE

test: all
	cd tests ; $(MAKE) check
