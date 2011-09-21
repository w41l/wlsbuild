## Process this file with automake to produce Makefile.in

## Created by Anjuta
SUBDIRS = libgmlib

DEFINES = -DMOZILLA_STRICT_API -DXP_UNIX
INCLUDES = \
	-fPIC \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GECKO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(GCONF_CFLAGS) \
	$(GCONF_DEFINES) \
	$(DEFINES) \
	$(CURL_DEFINES) \
	$(CURL_CFLAGS) 

AM_CFLAGS =\
	 -Wall \
	 -g 

COMMONCODE = \
	../config.h \
	plugin.cpp \
	memmem_compat.h \
	memmem_compat.cpp \
	plugin.h \
	plugin_list.h \
	plugin_list.cpp \
	plugin_types.h \
	plugin_setup.h \
	plugin_setup.cpp \
	plugin_dbus.h \
	plugin_dbus.cpp \
	np_entry.cpp \
	npn_gate.cpp \
	npp_gate.cpp \
	npupp.h

install_libexecdir = $(plugindir)
install_libexec_PROGRAMS = \
	gecko-mediaplayer.so \
	gecko-mediaplayer-qt.so \
	gecko-mediaplayer-wmp.so \
	gecko-mediaplayer-rm.so \
	gecko-mediaplayer-dvx.so
	
gecko_mediaplayer_so_SOURCES = \
	plugin_types_std.cpp \
	$(COMMONCODE)
		
gecko_mediaplayer_so_CFLAGS = -fPIC 
gecko_mediaplayer_so_LDFLAGS = -shared -fPIC
gecko_mediaplayer_so_LDADD = -ldl libgmlib/libgmlib.a $(GLIB_LIBS) $(DBUS_LIBS) $(GCONF_LIBS) $(CURL_LIBS) $(GIO_LIBS)

gecko_mediaplayer_qt_so_SOURCES = \
	plugin_types_qt.cpp \
	$(COMMONCODE)
	
gecko_mediaplayer_qt_so_CFLAGS = -fPIC 
gecko_mediaplayer_qt_so_LDFLAGS = -shared -fPIC
gecko_mediaplayer_qt_so_LDADD = -ldl libgmlib/libgmlib.a $(GLIB_LIBS) $(DBUS_LIBS) $(GCONF_LIBS) $(CURL_LIBS) $(GIO_LIBS)

gecko_mediaplayer_wmp_so_SOURCES = \
	plugin_types_wmp.cpp \
	$(COMMONCODE)
	
gecko_mediaplayer_wmp_so_CFLAGS = -fPIC 
gecko_mediaplayer_wmp_so_LDFLAGS = -shared -fPIC 
gecko_mediaplayer_wmp_so_LDADD = -ldl libgmlib/libgmlib.a $(GLIB_LIBS) $(DBUS_LIBS) $(GCONF_LIBS) $(CURL_LIBS) $(GIO_LIBS)

gecko_mediaplayer_dvx_so_SOURCES = \
	plugin_types_dvx.cpp \
	$(COMMONCODE)
	
gecko_mediaplayer_dvx_so_CFLAGS = -fPIC 
gecko_mediaplayer_dvx_so_LDFLAGS = -shared -fPIC
gecko_mediaplayer_dvx_so_LDADD = -ldl libgmlib/libgmlib.a $(GLIB_LIBS) $(DBUS_LIBS) $(GCONF_LIBS) $(CURL_LIBS) $(GIO_LIBS)

gecko_mediaplayer_rm_so_SOURCES = \
	plugin_types_rm.cpp \
	$(COMMONCODE)
	
gecko_mediaplayer_rm_so_CFLAGS = -fPIC 
gecko_mediaplayer_rm_so_LDFLAGS = -shared -fPIC
gecko_mediaplayer_rm_so_LDADD = -ldl libgmlib/libgmlib.a $(GLIB_LIBS) $(DBUS_LIBS) $(GCONF_LIBS) $(CURL_LIBS) $(GIO_LIBS)

indent:
	indent -kr -l100 -i4 -nut *.cpp
	indent -kr -l100 -i4 -nut *.h
	rm *~
