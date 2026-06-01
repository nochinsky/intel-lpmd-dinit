## Process this file with automake to generate Makefile.in

include $(GLIB_MAKEFILE)

# D-Bus system service activation
servicedir = $(datadir)/dbus-1/system-services
service_in_files = org.freedesktop.intel_lpmd.service.in
service_DATA = $(service_in_files:.service.in=.service)

$(service_DATA): $(service_in_files) Makefile
	@$(edit) $< >$@

# D-Bus security policy
dbusservicedir = $(DBUS_SYS_DIR)
dbusservice_DATA = org.freedesktop.intel_lpmd.conf

# Dinit service
dinitdir = $(datadir)/dinit.d
dinit_DATA = intel_lpmd.dinit

edit = sed \
	-e 's|@bindir[@]|$(bindir)|g' \
	-e 's|@sbindir[@]|$(sbindir)|g' \
	-e 's|@sysconfdir[@]|$(sysconfdir)|g' \
	-e 's|@localstatedir[@]|$(localstatedir)|g'

# Config files
lpmd_configdir = $(lpmd_confdir)
lpmd_config_DATA = \
	  intel_lpmd_config.xml \
	  intel_lpmd_config_F6_M170.xml \
	  intel_lpmd_config_F6_M189.xml \
	  intel_lpmd_config_F6_M204.xml

EXTRA_DIST = \
	intel_lpmd.dinit \
	org.freedesktop.intel_lpmd.service.in \
	$(dbusservice_DATA)

CLEANFILES = org.freedesktop.intel_lpmd.service