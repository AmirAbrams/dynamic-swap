packages:=boost openssl libevent zeromq gmp chia_bls backtrace
native_packages := native_ccache

qt_packages = qrencode zlib

qt_x86_64_linux_packages:=qt5.7 expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans
qt_i686_linux_packages:=$(qt_x86_64_linux_packages)

qt_darwin_packages=qt5.9
qt_mingw32_packages=qt5.9

wallet_packages=bdb

upnp_packages=miniupnpc

darwin_native_packages = native_biplist native_ds_store native_mac_alias

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif
