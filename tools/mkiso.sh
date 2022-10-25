	rm -rf build/iso_root
	mkdir -p build/iso_root
	cp build/psc/amd64/vxkern psc/amd64/limine.cfg \
		limine/limine.sys limine/limine-cd.bin \
		limine/limine-cd-efi.bin build/iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		build/iso_root -o build/barebones.iso
	build/limine-deploy build/barebones.iso
	rm -rf build/iso_root
