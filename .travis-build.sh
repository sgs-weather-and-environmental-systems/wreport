#!/bin/bash
set -ex

image=$1

if [[ $image =~ ^centos: ]]
then
    sed -i '/^tsflags=/d' /etc/yum.conf
    yum install -y epel-release
    yum install -y @buildsys-build
    yum install -y yum-utils
    yum install -y git
    yum-builddep -y fedora/SPECS/wreport.spec
elif [[ $image =~ ^fedora: ]]
then
    sed -i '/^tsflags=/d' /etc/dnf/dnf.conf
    dnf install -y @buildsys-build
    dnf install -y 'dnf-command(builddep)'
    dnf install -y git
    dnf builddep -y fedora/SPECS/wreport.spec
elif [[ $image =~ ^debian: ]]
then
    apt-get update -y
    apt-get install -y build-essential fakeroot devscripts
    apt-get build-dep -y debian/control
fi


if [[ $image =~ ^fedora: || $image =~ ^centos: ]]
then
    pkgname=wreport-$(git describe --abbrev=0 --tags --match='v*' | sed -e 's,^v,,g')
    mkdir -p ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
    cp fedora/SPECS/wreport.spec ~/rpmbuild/SPECS/wreport.spec
    git archive --prefix=$pkgname/ --format=tar HEAD | gzip -c > ~/rpmbuild/SOURCES/$pkgname.tar.gz
    rpmbuild -ba ~/rpmbuild/SPECS/wreport.spec
    find ~/rpmbuild/{RPMS,SRPMS}/ -name "${pkgname}*rpm" -exec cp -v {} . \;
    # TODO upload ${pkgname}*.rpm to github release on deploy stage
elif [[ $image =~ ^debian: ]]
then
    debuild -b -uc -us
else
    autoreconf -ifv
    ./configure
    make check
fi
