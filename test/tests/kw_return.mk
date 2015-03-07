all: test_kw_return

.PHONY: test_kw_return
test_kw_return: STEP=test_kw_return
test_kw_return: $(PASS)/out.log.test_kw_return $(PASS)/err.log.test_kw_return
$(PASS)/out.log.test_kw_return $(PASS)/err.log.test_kw_return: src/kw_return.c src/kw_return2.c $(TGT) 
	@echo Test $(STEP): Token/Statement Count: Return
	$(TGT_E) $(EXCLUDE) src/kw_return.c src/kw_return2.c  --output-metrics=STMT_RETURN_CNT,TOK_RETURN,RETURN_POINT_CNT -- $(OUT)
	$(CHECK_E)
