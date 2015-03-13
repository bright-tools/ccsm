all: test_fn_call_single_file

.PHONY: test_fn_call_single_file
test_fn_call_single_file: STEP=test_fn_call_single_file
test_fn_call_single_file: METRICS=FUNC_CALLED_BY,LOCAL_FN_CALL_CNT,LOCAL_FN_CALL_CNT,OP_FN_CALL_CNT,OP_FN_CALL_UNIQUE_CNT
test_fn_call_single_file: FILES=src/fn_call.c
test_fn_call_single_file: $(PASS)/out.log.test_fn_call_single_file $(PASS)/err.log.test_fn_call_single_file
$(PASS)/out.log.test_fn_call_single_file $(PASS)/err.log.test_fn_call_single_file: src/fn_call.c $(TGT) 
	@echo Test $(STEP): Function call count \(single file\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

