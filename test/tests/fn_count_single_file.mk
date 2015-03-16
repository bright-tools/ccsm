all: test_fn_count_single_file

.PHONY: test_fn_count_single_file
test_fn_count_single_file: STEP=test_fn_count_single_file
test_fn_count_single_file: METRICS=FUNC_CNT
test_fn_count_single_file: FILES=src/fn_call.c
test_fn_count_single_file: $(PASS)/out.log.test_fn_count_single_file $(PASS)/err.log.test_fn_count_single_file
$(PASS)/out.log.test_fn_count_single_file $(PASS)/err.log.test_fn_count_single_file: src/fn_call.c $(TGT)
	@echo Test $(STEP): Function count \(single file\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

