all: test_include

.PHONY: test_include
test_include: STEP=test_include
test_include: METRICS=VAR_FILE_LOC_CNT,VAR_FILE_LOC_STATIC_CNT,VAR_FILE_EXT_CNT,VAR_FN_LOC_CNT,VAR_FN_LOC_STATIC_CNT,VAR_FN_LOC_REG_CNT,VAR_FN_LOC_AUTO_CNT,VAR_FN_EXT_CNT
test_include: FILES=src/simple_include.c
test_include: $(PASS)/out.log.test_include $(PASS)/err.log.test_include
$(PASS)/out.log.test_include $(PASS)/err.log.test_include: src/simple_include.c $(TGT)
	@echo Test $(STEP): Simple test of include files
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
