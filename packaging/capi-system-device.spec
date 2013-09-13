Name:       capi-system-device
Summary:    A Device library in TIZEN C API
Version:    0.1.0
Release:    19
Group:      System/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    capi-system-device.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)

Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A Device library in TIZEN C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
cp %{SOURCE1} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE.APLv2 %{buildroot}%{_datadir}/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-system-device.manifest
%{_libdir}/libcapi-system-device.so.*
%{_datadir}/license/%{name}

%files devel
%{_includedir}/system/device.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-device.so
