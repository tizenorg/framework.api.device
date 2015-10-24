Name:       capi-system-device
Summary:    A Device library in TIZEN C API
Version:    0.1.0
Release:    18
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    capi-system-device.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(gio-2.0)

Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description
A Device library in TIZEN C API package.

%package devel
Summary:  A Device library in TIZEN C API (Development)
Group:    System/Development
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q

%build
%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS+=" -DTIZEN_ENGINEER_MODE"
%endif
cp %{SOURCE1} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%{_libdir}/libcapi-system-device.so.*
%{_datadir}/license/%{name}

%files devel
%manifest %{name}.manifest
%{_includedir}/device/*.h
%{_includedir}/system/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-device.so
