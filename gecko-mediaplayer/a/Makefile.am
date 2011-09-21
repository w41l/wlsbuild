## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

gecko_mediaplayerdocdir = ${docdir}
gecko_mediaplayerdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	DOCS/tech/javascript.txt

if GCONF_SCHEMAS_INSTALL
schemadir   = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = gecko-mediaplayer.schemas
endif

EXTRA_DIST = $(gecko_mediaplayerdoc_DATA) $(schema_DATA) gecko-mediaplayer.spec.in gecko-mediaplayer.schemas

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

install-schemas: $(schema_DATA)
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) gconftool-2 --makefile-install-rule $(schema_DATA) 2>&1 > /dev/null

if GCONF_SCHEMAS_INSTALL
install-data-hook: install-schemas
else
install-data-hook:
endif

