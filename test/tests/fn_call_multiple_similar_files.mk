all: test_fn_call_multiple_similar_files

.PHONY: test_fn_call_multiple_similar_files
test_fn_call_multiple_similar_files: STEP=test_fn_call_multiple_similar_files
test_fn_call_multiple_similar_files: METRICS=TOK_MINUSMINUS,TOK_PLUSPLUS,FUNC_CALLED_BY_LOCAL,FUNC_CALLED_BY_EXTERN,OP_FN_CALL_CNT,OP_FN_CALL_UNIQUE_CNT
test_fn_call_multiple_similar_files: FILES=src/fn_call_similar1.c src/fn_call_similar2.c
test_fn_call_multiple_similar_files: $(PASS)/out.log.test_fn_call_multiple_similar_files $(PASS)/err.log.test_fn_call_multiple_similar_files
$(PASS)/out.log.test_fn_call_multiple_similar_files $(PASS)/err.log.test_fn_call_multiple_similar_files: src/fn_call_similar1.c src/fn_call_similar2.c $(TGT)
	@echo Test $(STEP): Function call count \(multiple similar files\)
	$(TGT_E) $(EXCLUDE) $(FILES) --output-metrics=$(METRICS) --dump-function-map -- $(OUT)
	$(CHECK_E)

