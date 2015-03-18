all: test_func_path

.PHONY: test_func_path
test_func_path: STEP=test_func_path
test_func_path: METRICS=FUNC_PATHS
test_func_path: FILES=src/func_path.c src/func_path2.c src/nesting.c src/nesting2.c
test_func_path: $(PASS)/out.log.test_func_path $(PASS)/err.log.test_func_path
$(PASS)/out.log.test_func_path $(PASS)/err.log.test_func_path: src/func_path.c src/func_path2.c src/nesting.c src/nesting2.c $(TGT)
	@echo Test $(STEP): Function paths
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
