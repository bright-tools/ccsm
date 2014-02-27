PFX     = 
TGT     = /cygdrive/C/Users/catalyst/Documents/coverage/build4/bin/Debug/ccsm.exe
LOG_OUT = out.log
LOG_OUT_T = out.log.tmp
LOG_ERR = err.log
LOG_ERR_T = err.log.tmp
PP_ERR  = sed -e 's/\s.*src/ src/;' > $(LOG_ERR)
PP_OUT  = sed -e 's/\s.*src/ src/;' > $(LOG_OUT)
OUT     = 2>$(LOG_ERR_T) 1>$(LOG_OUT_T) && cat $(LOG_ERR_T) | $(PP_ERR) && cat $(LOG_OUT_T) | $(PP_OUT)
TGT_E   = $(PFX) $(TGT)
DIFF_OPT= -c -b
PASS    = expected
CHECK_E = $(PFX) diff $(DIFF_OPT) out.log $(PASS)/out.log.$(STEP) && diff $(DIFF_OPT) err.log $(PASS)/err.log.$(STEP)

.PHONY: all
all: test1
	@echo -e \\nAll tests run OK

.PHONY: clean
clean:
	@echo Cleaning up
	$(PFX) rm -rf $(LOG_OUT) $(LOG_ERR) $(LOG_ERR_T)

.PHONY: test1
test1: STEP=1
test1: clean
	@echo Test $(STEP): Single C file
	$(TGT_E) src/1.c -- $(OUT)
	$(CHECK_E)

.PHONY: test3
test1: STEP=1
test2: test2
	@echo Test $(STEP): Single C++ file
	$(TGT_E) src/2.cpp -- $(OUT)
	$(CHECK_E)

.PHONY: test3
test1: STEP=1
test2: test2
	@echo Test $(STEP): Multiple file
	$(TGT_E) src/1.c src/2.cpp -- $(OUT)
	$(CHECK_E)

