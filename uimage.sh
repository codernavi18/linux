ARCH=arm64
CROSS_COMPILE=/opt/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

#step1 : clean the existing images
rm /home/naveen/tftpboot/hi6220-hikey.dtb /home/naveen/tftpboot/uImage

#step2 : prepare the uimage
${CROSS_COMPILE}objcopy -O binary -R .note -R .comment -S vmlinux linux.bin
mkimage -A arm64 -O linux -T kernel -C none -a 0x80000 -e 0x80000 -n "Linux Kernel Image" -d linux.bin uImage

#step3 : copy the images to tftpboot
sudo cp uImage /home/naveen/tftpboot
sudo cp arch/arm64/boot/dts/hisilicon/hi6220-hikey.dtb /home/naveen/tftpboot
