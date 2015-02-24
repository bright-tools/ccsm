all: test_fn_count_single_file

.PHONY: test_fn_count_single_file
test_fn_count_single_file: STEP=test_fn_count_single_file
test_fn_count_single_file: $(PASS)/out.log.test_fn_count_single_file $(PASS)/err.log.test_fn_count_single_file
$(PASS)/out.log.test_fn_count_single_file $(PASS)/err.log.test_fn_count_single_file: src/fn_call.c $(TGT) 
	@echo Test $(STEP): Function call count \(single file\)
	$(TGT_E) $(EXCLUDE) src/fn_call.c --output-metrics=FUNC_CNT -- $(OUT)
	$(CHECK_E)

