all: fn_call_multiple_files

.PHONY: test_fn_call_multiple_files
fn_call_multiple_files: STEP=test_fn_call_multiple_files
fn_call_multiple_files: $(PASS)/out.log.test_fn_call_multiple_files $(PASS)/err.log.test_fn_call_multiple_files
$(PASS)/out.log.fn_call_multiple_files $(PASS)/err.log.fn_call_multiple_files: src/fn_call.c src/fn_call2.c $(TGT) 
	@echo Test $(STEP): Function call count \(single file\)
	$(TGT_E) $(EXCLUDE) src/fn_call.c src/fn_call2.c --output-metrics=FUNC_CALLED_BY,OP_FN_CALL* -- $(OUT)
	$(CHECK_E)

