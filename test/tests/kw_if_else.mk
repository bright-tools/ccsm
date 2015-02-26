all: test_kw_if_else

.PHONY: test_kw_if_else
test_kw_if_else: STEP=test_kw_if_else
test_kw_if_else: $(PASS)/out.log.test_kw_if_else $(PASS)/err.log.test_kw_if_else
$(PASS)/out.log.test_kw_if_else $(PASS)/err.log.test_kw_if_else: src/kw_if_else.c $(TGT) 
	@echo Test $(STEP): Token/Statement Count: If/Else
	$(TGT_E) $(EXCLUDE) src/kw_if_else.c  --output-metrics=STMT_IF_CNT,TOK_IF,STMT_ELSE_CNT,TOK_ELSE -- $(OUT)
	$(CHECK_E)

