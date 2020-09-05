Here's how to build original u-boot for 

`export STAGING_DIR=~/wrtbuilder/openwrt/staging_dir/toolchain-mips_24kc_gcc-8.4.0_musl/bin/`

`export PATH=$STAGING_DIR:$PATH`

`git clone https://github.com/u-boot/u-boot`

`git checkout v2020.07`

`make ap143_defconfig`

`make CROSS_COMPILE=mips-openwrt-linux-`


