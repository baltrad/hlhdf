%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}
%define _prefix /opt/baltrad/%{name}

Name: hlhdf
Version: 0.8.9
Release: %{snapshot}%{?dist}
Summary: HL-HDF
License: GPL-3 and LGPL-3
URL: http://www.baltrad.eu/
Source0: %{name}-%{version}.tar.gz
Source1: hlhdf-python.conf
BuildRequires: hdf5-devel
Requires: hdf5
BuildRequires: zlib-devel
BuildRequires: python2-devel
BuildRequires: numpy
BuildRequires: atlas

%description
A High Level Interface to the HDF5 File Format

%package devel
Summary: HL-HDF development files
Group: Development/Libraries
Requires: %{name} = %{version}
Requires: hdf5-devel

%description devel
HL-HDF development headers and libraries.

%package python
Summary: HL-HDF Python bindings
Requires: %{name} = %{version}

%description python
HL-HDF Python bindings

%prep
%setup -q
#%patch1 -p1


%build
%configure
make

%install
# FIXME: Why is this mkdir necessary?
# With full _prefix the custom installscripts think there was already an old version
# present and does some special things we may not want (migration to newer version)
mkdir -p %{buildroot}/opt/baltrad
#mkdir -p %{buildroot}%{_sysconfdir}/ld.so.conf.d

make install DESTDIR=%{buildroot}
mkdir -p %{buildroot}%{python_sitearch}
mv %{buildroot}%{_prefix}/lib/_pyhl.so %{buildroot}%{python_sitearch}/
install -p -D -m 0644 %{SOURCE1} %{buildroot}%{_sysconfdir}/ld.so.conf.d/hlhdf-python.conf
mkdir -p %{buildroot}/usr/lib
ln -sf ../../opt/baltrad/hlhdf/lib/libhlhdf.so %{buildroot}/usr/lib/libhlhdf.so

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_prefix}/bin/hldec
%{_prefix}/bin/hlenc
%{_prefix}/bin/hlinstall.sh
%{_prefix}/bin/hllist
%{_prefix}/lib/libhlhdf.so
%{_prefix}/mkf/hldef.mk
%{_prefix}/hlhdf.pth
%{_prefix}/bin
%{_prefix}/lib
%{_prefix}/mkf
/usr/lib/libhlhdf.so

%files python
%{python_sitearch}/_pyhl.so
%config(noreplace) %{_sysconfdir}/ld.so.conf.d/hlhdf-python.conf

%files devel
%{_prefix}/include/hlhdf.h
%{_prefix}/include/hlhdf_alloc.h
%{_prefix}/include/hlhdf_arrayobject_wrap.h
%{_prefix}/include/hlhdf_compound.h
%{_prefix}/include/hlhdf_compound_utils.h
%{_prefix}/include/hlhdf_debug.h
%{_prefix}/include/hlhdf_node.h
%{_prefix}/include/hlhdf_nodelist.h
%{_prefix}/include/hlhdf_read.h
%{_prefix}/include/hlhdf_types.h
%{_prefix}/include/hlhdf_write.h
%{_prefix}/include/pyhlhdf_common.h
%{_prefix}/include/pyhlcompat.h
%{_prefix}/include
%{_prefix}/lib/libhlhdf.a
%{_prefix}/lib/libpyhlhdf.a

