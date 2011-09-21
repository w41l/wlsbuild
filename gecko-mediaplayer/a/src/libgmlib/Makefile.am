## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = \
        libgmlib.a


libgmlib_a_SOURCES = \
   	gmlib.h \
   	gm_audio.h \
   	gm_audio.c \
   	gm_file.h \
   	gm_file.c \
   	gm_parse.h \
   	gm_parse.c \
	gm_pref_store.h \
	gm_pref_store.c \
	gm_strfuncs.h \
	gm_strfuncs.c

libgmlib_a_CFLAGS = -fPIC \
	$(GLIB_CFLAGS) \
	$(GCONF_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GCONF_DEFINES) \
	$(GIO_DEFINES) \
	$(GLIB2_26_DEFINES) \
	$(ALSA_CFLAGS) \
	$(PULSE_CFLAGS)

indent:
	indent -kr -l120 -i4 -nut *.c
	indent -kr -l120 -i4 -nut *.h
	rm *~
	
cppcheck:
	cppcheck -q -a -v -f -j3 .
	
