all: test_nesting

.PHONY: test_nesting
test_nesting: STEP=test_nesting
test_nesting: METRICS=FUNC_NESTING
test_nesting: FILES=src/nesting.c src/nesting2.c
test_nesting: $(PASS)/out.log.test_nesting $(PASS)/err.log.test_nesting
$(PASS)/out.log.test_nesting $(PASS)/err.log.test_nesting: src/nesting.c src/nesting2.c $(TGT)
	@echo Test $(STEP): Function Nesting
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
