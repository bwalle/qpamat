Summary: 	Password manager written with Qt
Name: 		qpamat
Version: 	0.3.3
Release: 	0
Packager:       Bernhard Walle <bernhard@bwalle.de>
License:	GPL
URL: 		http://www.bwalle.de/temp/qpamat/
Source0: 	%{name}-%{version}.tar.gz
Group: 		Utilties
BuildRoot: 	%{_tmppath}/%{name}-root
Requires:	qt3 >= 3.2.0 
Requires:	openssl >= 0.9.7
BuildRequires:	qt3-devel >= 3.2.0
BuildRequires: 	openssl-devel >= 0.9.7

%description
Tool for managing passwords. The passwords are saved on the
harddisk or on a smartcard. Security is provided by the
OpenSSL crypto library. The User Interface is written with
Qt from Trolltech.

Author
------
  Bernhard Walle <bernhard@bwalle.de>

%prep
%setup -q

%build
qmake -o Makefile qpamat.pro
lrelease qpamat.pro
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_bindir} 
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/qpamat
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/qpamat/translations
install -m 0755 qpamat $RPM_BUILD_ROOT/%{_bindir}
cp -R share/dicts $RPM_BUILD_ROOT/%{_datadir}/qpamat
install -m 0644 COPYING $RPM_BUILD_ROOT/%{_datadir}/qpamat/
install -m 0644 ts/*.qm $RPM_BUILD_ROOT/%{_datadir}/qpamat/translations
ln -s %{_docdir}/qpamat $RPM_BUILD_ROOT/%{_datadir}/qpamat/doc

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README COPYING doc/qpamat.dtd doc/user/en
%{_bindir}/qpamat
%{_datadir}/qpamat
