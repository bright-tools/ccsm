all: test_fn_call_multiple_files

.PHONY: test_fn_call_multiple_files
test_fn_call_multiple_files: STEP=test_fn_call_multiple_files
test_fn_call_multiple_files: METRICS=HIS_CALLING,FUNC_CALLED_BY_LOCAL,FUNC_CALLED_BY_EXTERN,LOCAL_FN_CALL_CNT,OP_FN_CALL_CNT,OP_FN_CALL_UNIQUE_CNT
test_fn_call_multiple_files: FILES=src/fn_call.c src/fn_call2.c
test_fn_call_multiple_files: $(PASS)/out.log.test_fn_call_multiple_files $(PASS)/err.log.test_fn_call_multiple_files
$(PASS)/out.log.test_fn_call_multiple_files $(PASS)/err.log.test_fn_call_multiple_files: src/fn_call.c src/fn_call2.c $(TGT)
	@echo Test $(STEP): Function call count \(multiple files\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

