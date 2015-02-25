all: test_fn_call_multiple_similar_files

.PHONY: test_fn_call_multiple_similar_files
test_fn_call_multiple_similar_files: STEP=test_fn_call_multiple_similar_files
test_fn_call_multiple_similar_files: $(PASS)/out.log.test_fn_call_multiple_similar_files $(PASS)/err.log.test_fn_call_multiple_similar_files
$(PASS)/out.log.test_fn_call_multiple_similar_files $(PASS)/err.log.test_fn_call_multiple_similar_files: src/fn_call_similar1.c src/fn_call_similar2.c $(TGT) 
	@echo Test $(STEP): Function call count \(multiple similar files\)
	$(TGT_E) $(EXCLUDE)  src/fn_call_similar1.c src/fn_call_similar2.c --output-metrics=TOK_MINUSMINUS,TOK_PLUSPLUS,FUNC_CALLED_BY,OP_FN_CALL* --dump-function-map -- $(OUT)
	$(CHECK_E)

