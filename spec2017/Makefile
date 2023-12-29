SPECKLE  ?= ~/proj/speckle-2017
SPEC_DIR = ../rootfs/buildroot_initramfs_sysroot/spec

intrate_benches = \
	500.perlbench_r \
	502.gcc_r \
	505.mcf_r \
	520.omnetpp_r \
	523.xalancbmk_r \
	531.deepsjeng_r \
	541.leela_r \
	548.exchange2_r \
	557.xz_r

#   525.x264_r \
#   this is too large. The case need to decode a 1.7GB video (BuckBunny.yuv), which is too slow to put in Spike

fprate_benches = \
	503.bwaves_r \
	508.namd_r \
	511.povray_r \
	521.wrf_r \
	527.cam4_r \
	544.nab_r \
	554.roms_r \
	507.cactuBSSN_r \
	510.parest_r \
	519.lbm_r \
	526.blender_r \
	538.imagick_r \
	549.fotonik3d_r

intrate_cases = $(patsubst %, bbl-%, $(intrate_benches))
fprate_cases  = $(patsubst %, bbl-%, $(fprate_benches))

all_cases = $(intrate_cases) $(fprate_cases)

.PHONY: all
all: $(SPECKLE) $(all_cases)

$(intrate_cases): bbl-%: $(SPECKLE)/build/overlay/intrate/% ../rootfs/buildroot_initramfs_sysroot
	-rm -fr $(SPEC_DIR)
	cp -pr $< $(SPEC_DIR)
	$(MAKE) -C ../ bbl
	mv ../build/riscv-pk/bbl $@

$(fprate_cases): bbl-%: $(SPECKLE)/build/overlay/fprate/% ../rootfs/buildroot_initramfs_sysroot
	-rm -fr $(SPEC_DIR)
	cp -pr $< $(SPEC_DIR)
	$(MAKE) -C ../ bbl
	mv ../build/riscv-pk/bbl $@

.PHONY: clean
clean:
	-rm -fr $(SPEC_DIR)
	-rm $(all_cases)
