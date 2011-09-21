# norootforbuild

Name:           0ad
Version:        r09786
Release:        1.0
License:        GNU GPL v2 or later
Group:          Amusements/Games/Strategy/Real Time
Url:            http://wildfiregames.com/0ad/
Source:         0ad-%{version}-alpha-unix-build.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  boost-devel
BuildRequires:  DevIL-devel
BuildRequires:  fam-devel
BuildRequires:  gcc-c++
BuildRequires:  libcurl-devel
BuildRequires:  libjpeg-devel
BuildRequires:  libpng-devel
BuildRequires:  libvorbis-devel
BuildRequires:  libxml2-devel
BuildRequires:  nasm
BuildRequires:  python
BuildRequires:  subversion
BuildRequires:  zip
BuildRequires:  cmake

%if 0%{?suse_version}
BuildRequires:  update-desktop-files
BuildRequires:  libenet-devel
BuildRequires:  openal-soft-devel
BuildRequires:  pkg-config

%if 0%{?suse_version} >= 1140
BuildRequires:  wxWidgets-wxcontainer-devel
%define _use_internal_dependency_generator 0
%define __find_requires %wx_requires
%else
BuildRequires:  wxGTK-devel
%endif

%if 0%{?suse_version} == 1110
BuildRequires:  SDL-devel
%else
BuildRequires:  libSDL-devel
%endif

%endif

%if 0%{?fedora_version}
BuildRequires:  desktop-file-utils
BuildRequires:  libenet-devel
BuildRequires:  openal-soft-devel
BuildRequires:  pkgconfig
BuildRequires:  SDL-devel
BuildRequires:  wxGTK-devel
%endif

%if 0%{?mandriva_version}
BuildRequires:  libenet-devel
BuildRequires:  libwxgtku2.8-devel

%if 0%{?mandriva_version} < 201000
BuildRequires:  libopenal-devel
%else:
BuildRequires:  openal-soft-devel
%endif

%if 0%{?mandriva_version} == 201000
BuildRequires:  libopenal1-soft
%endif

%endif

Requires:       0ad-data
Summary:        Free, Open-Source, Cross-Platform RTS Game of Ancient Warfare

%description
0 A.D. (pronounced "zero ey-dee") is a free, open-source, cross-platform real-time 
strategy (RTS) game of ancient warfare. In short, it is a historically-based 
war/economy game that allows players to relive or rewrite the history of Western 
civilizations, focusing on the years between 500 B.C. and 500 A.D. The project is 
highly ambitious, involving state-of-the-art 3D graphics, detailed artwork, sound, 
and a flexible and powerful custom-built game engine.

The game has been in development by Wildfire Games (WFG), a group of volunteer, 
hobbyist game developers, since 2001. The code and data are available under the GPL 
license, and the art, sound and documentation are available under CC-BY-SA. In short, 
we consider 0 A.D. an an educational celebration of game development and ancient 
history.


%prep
%setup -q -n %{name}-%{version}-alpha

%build
export CFLAGS="%{optflags}"
export CPPFLAGS="%{optflags}"
build/workspaces/update-workspaces.sh --verbose --bindir %{_bindir} --datadir %{_datadir}/%{name} --libdir %{_libdir}/%{name} --with-system-enet
pushd build/workspaces/gcc
%__make CONFIG=Release %{?_smp_mflags}
popd


%check
#__make check

# Disabled for now because of weird failures on OpenSUSE 11.3 i586
# (looks like SpiderMonkey doesn't like the OBS environment or something?)
#LD_LIBRARY_PATH=binaries/system binaries/system/test -libdir binaries/system

%install
#makeinstall
%__install -Dm 0755 binaries/system/pyrogenesis %{buildroot}%{_bindir}/pyrogenesis
%__install -Dm 0755 binaries/system/libCollada.so %{buildroot}%{_libdir}/%{name}/libCollada.so
%__install -Dm 0755 binaries/system/libAtlasUI.so %{buildroot}%{_libdir}/%{name}/libAtlasUI.so
%__install -Dm 0755 binaries/system/libmozjs-ps-release.so %{buildroot}%{_libdir}/%{name}/libmozjs-ps-release.so
%__install -Dm 0755 binaries/system/libnvcore.so %{buildroot}%{_libdir}/%{name}/libnvcore.so
%__install -Dm 0755 binaries/system/libnvimage.so %{buildroot}%{_libdir}/%{name}/libnvimage.so
%__install -Dm 0755 binaries/system/libnvmath.so %{buildroot}%{_libdir}/%{name}/libnvmath.so
%__install -Dm 0755 binaries/system/libnvtt.so %{buildroot}%{_libdir}/%{name}/libnvtt.so

#__install -Dm 0755 binaries/system/ActorEditor %{buildroot}/%{_libexecdir}/%{name}/bin/ActorEditor
#__install -Dm 0755 binaries/system/ColourTester %{buildroot}/%{_libexecdir}/%{name}/bin/ColourTester

%__install -Dm 0644 build/resources/0ad.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
%__install -Dm 0644 build/resources/0ad.png %{buildroot}%{_datadir}/pixmaps/%{name}.png

%if 0%{?suse_version}
%suse_update_desktop_file %{name}
%endif

%if 0%{?fedora_version}
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop
%endif

%__install -Dm 0755 build/resources/0ad.sh %{buildroot}%{_bindir}/0ad

%__mkdir_p %{buildroot}%{_libdir}/%{name}
%__mkdir_p %{buildroot}%{_datadir}/%{name}

%clean
test "%{buildroot}" != "/" && %__rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc README.txt
%{_bindir}/0ad
%{_bindir}/pyrogenesis
%{_libdir}/%{name}/libCollada.so
%{_libdir}/%{name}/libAtlasUI.so
%{_libdir}/%{name}/libmozjs-ps-release.so
%{_libdir}/%{name}/libnvcore.so
%{_libdir}/%{name}/libnvimage.so
%{_libdir}/%{name}/libnvmath.so
%{_libdir}/%{name}/libnvtt.so
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/applications/%{name}.desktop
%dir %{_libdir}/%{name}
%dir %{_datadir}/%{name}
