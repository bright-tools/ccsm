all: test_two_include

.PHONY: test_two_include
test_two_include: STEP=test_two_include
test_two_include: METRICS=VAR_FILE_LOC_CNT,VAR_FILE_LOC_STATIC_CNT,VAR_FILE_EXT_CNT,VAR_FN_LOC_CNT,VAR_FN_LOC_STATIC_CNT,VAR_FN_LOC_REG_CNT,VAR_FN_LOC_AUTO_CNT,VAR_FN_EXT_CNT
test_two_include: FILES=src/simple_include.c src/second_include.c
test_two_include: $(PASS)/out.log.test_two_include $(PASS)/err.log.test_two_include
$(PASS)/out.log.test_two_include $(PASS)/err.log.test_two_include: src/simple_include.c src/second_include.c $(TGT)
	@echo Test $(STEP): Test of same header file included in 2 source files
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

