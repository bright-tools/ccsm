all: test_stress
clean: test_stress_clean

BACNET_SOURCE_ZIP=bacnet-stack-0.8.2.zip
BACNET_DIR=bacnet-stack-0.8.2

.PHONY: test_stress_clean
test_stress_clean:
	if [ -f $(BACNET_SOURCE_ZIP) ]; then rm $(BACNET_SOURCE_ZIP); fi

.PHONY: test_stress
test_stress: STEP=test_stress
test_stress: $(PASS)/out.log.test_stress $(PASS)/err.log.test_stress
$(PASS)/out.log.test_stress $(PASS)/err.log.test_stress: $(TGT)
	@echo Test $(STEP): Stress Test \(BACnet source code\)
	$(PFX) if [ ! -f $(BACNET_SOURCE_ZIP) ]; then \
		curl -L http://sourceforge.net/projects/bacnet/files/bacnet-stack/bacnet-stack-0.8.2/bacnet-stack-0.8.2.zip/download -o $(BACNET_SOURCE_ZIP); \
	fi
	$(PFX) if [ ! -d $(BACNET_DIR) ]; then \
	    unzip $(BACNET_SOURCE_ZIP); \
	fi
	$(TGT_E) \
bacnet-stack-0.8.2/src/abort.c \
bacnet-stack-0.8.2/src/address.c \
bacnet-stack-0.8.2/src/alarm_ack.c \
bacnet-stack-0.8.2/src/apdu.c \
bacnet-stack-0.8.2/src/arf.c \
bacnet-stack-0.8.2/src/awf.c \
bacnet-stack-0.8.2/src/bacaddr.c \
bacnet-stack-0.8.2/src/bacapp.c \
bacnet-stack-0.8.2/src/bacdcode.c \
bacnet-stack-0.8.2/src/bacdevobjpropref.c \
bacnet-stack-0.8.2/src/bacerror.c \
bacnet-stack-0.8.2/src/bacint.c \
bacnet-stack-0.8.2/src/bacprop.c \
bacnet-stack-0.8.2/src/bacpropstates.c \
bacnet-stack-0.8.2/src/bacreal.c \
bacnet-stack-0.8.2/src/bacstr.c \
bacnet-stack-0.8.2/src/bactext.c \
bacnet-stack-0.8.2/src/bigend.c \
bacnet-stack-0.8.2/src/cov.c \
bacnet-stack-0.8.2/src/crc.c \
bacnet-stack-0.8.2/src/datetime.c \
bacnet-stack-0.8.2/src/dcc.c \
bacnet-stack-0.8.2/src/debug.c \
bacnet-stack-0.8.2/src/event.c \
bacnet-stack-0.8.2/src/fifo.c \
bacnet-stack-0.8.2/src/filename.c \
bacnet-stack-0.8.2/src/get_alarm_sum.c \
bacnet-stack-0.8.2/src/getevent.c \
bacnet-stack-0.8.2/src/iam.c \
bacnet-stack-0.8.2/src/ihave.c \
bacnet-stack-0.8.2/src/indtext.c \
bacnet-stack-0.8.2/src/key.c \
bacnet-stack-0.8.2/src/keylist.c \
bacnet-stack-0.8.2/src/lso.c \
bacnet-stack-0.8.2/src/memcopy.c \
bacnet-stack-0.8.2/src/mstp.c \
bacnet-stack-0.8.2/src/mstptext.c \
bacnet-stack-0.8.2/src/npdu.c \
bacnet-stack-0.8.2/src/proplist.c \
bacnet-stack-0.8.2/src/ptransfer.c \
bacnet-stack-0.8.2/src/rd.c \
bacnet-stack-0.8.2/src/readrange.c \
bacnet-stack-0.8.2/src/reject.c \
bacnet-stack-0.8.2/src/ringbuf.c \
bacnet-stack-0.8.2/src/rp.c \
bacnet-stack-0.8.2/src/rpm.c \
bacnet-stack-0.8.2/src/sbuf.c \
bacnet-stack-0.8.2/src/timestamp.c \
bacnet-stack-0.8.2/src/timesync.c \
bacnet-stack-0.8.2/src/version.c \
bacnet-stack-0.8.2/src/whohas.c \
bacnet-stack-0.8.2/src/whois.c \
bacnet-stack-0.8.2/src/wp.c \
bacnet-stack-0.8.2/src/wpm.c \
       --output-format=sparsetree -- -I$(BACNET_DIR)/include -I$(BACNET_DIR)/ports/linux $(OUT)
	$(CHECK_E)
