all: fn_count_multiple_files

.PHONY: test_fn_count_multiple_files
fn_count_multiple_files: STEP=test_fn_count_multiple_files
fn_count_multiple_files: $(PASS)/out.log.fn_count_multiple_files $(PASS)/err.log.fn_count_multiple_files
$(PASS)/out.log.fn_count_multiple_files $(PASS)/err.log.fn_count_multiple_files: src/fn_call.c src/fn_call2.c $(TGT) 
	@echo Test $(STEP): Function call count \(multiple file\)
	$(TGT_E) $(EXCLUDE) src/fn_call.c src/fn_call2.c --output-metrics=FUNC_CNT -- $(OUT)
	$(CHECK_E)

