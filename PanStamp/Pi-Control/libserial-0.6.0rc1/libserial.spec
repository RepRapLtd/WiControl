# If you want to build an executable that uses static libraries,
# build your libraries in a separate root directory, and then run
# the rpmbuild using "--define 'ink_static_root /your/static/rootdir'"

# To avoid stripping the binaries, you can also:
#  "--define '__spec_install_post /usr/lib/rpm/brp-compress'"

Name:		libserial
Summary:	C++ library for programming serial ports on POSIX systems.
Version:	0.6.0rc1
# Use release "0" so that distro-released versions will override ours.
Release:	0
License:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.gz
URL:		http://libserial.sourceforge.net/
Prefix:		%{_prefix}
Packager:	Automatic
Vendor:		GNU
BuildRoot:	%{_tmppath}/%{name}-%{version}-root

# See debian/control for a full list of requirements.  Maybe someone using an
# rpm-based distribution would like to translate some or all of the Debian
# package names given in debian/control to rpm equivalent packages.
#Requires: gtk2 >= 2.4.0
#BuildRequires: gtk2-devel >= 2.4.0

%description
A collection of C++ classes which allow the serial port on POSIX
systems to be accessed like an iostream object. Special functions are
provided for setting various parameters of the serial port such as the
baud rate, character size, flow control and others.

%prep
%setup

%build
### Needed for snapshot releases.
MYCFLAGS="$RPM_OPT_FLAGS"

if [ ! -x configure ]; then
  CFLAGS="$MYCFLAGS" ./autogen.sh $MYARCH_FLAGS --prefix=%{_prefix} --localstatedir=%{_localstatedir} --sysconfdir=%{_sysconfdir}
else
  %configure 
fi
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%makeinstall

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README 
%{_includedir}/SerialPort.h
%{_includedir}/SerialStream.h
%{_includedir}/SerialStreamBuf.h
%{_libdir}/libserial.a
%{_libdir}/libserial.la
%{_libdir}/libserial.so
%{_libdir}/libserial.so.0 
%{_libdir}/libserial.so.0.0.0

%changelog
