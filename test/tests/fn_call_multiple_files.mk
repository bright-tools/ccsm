all: test_fn_call_multiple_files

.PHONY: test_fn_call_multiple_files
test_fn_call_multiple_files: STEP=test_fn_call_multiple_files
test_fn_call_multiple_files: $(PASS)/out.log.test_fn_call_multiple_files $(PASS)/err.log.test_fn_call_multiple_files
$(PASS)/out.log.test_fn_call_multiple_files $(PASS)/err.log.test_fn_call_multiple_files: src/fn_call.c src/fn_call2.c $(TGT) 
	@echo Test $(STEP): Function call count \(multiple files\)
	$(TGT_E) $(EXCLUDE) src/fn_call.c src/fn_call2.c --output-metrics=FUNC_CALLED_BY,LOCAL_FN_CALL_CNT,OP_FN_CALL* -- $(OUT)
	$(CHECK_E)

