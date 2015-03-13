all: test_kw_if_else

.PHONY: test_kw_if_else
test_kw_if_else: STEP=test_kw_if_else
test_kw_if_else: METRICS=STMT_IF_CNT,TOK_IF,STMT_ELSE_CNT,TOK_ELSE
test_kw_if_else: FILES=src/kw_if_else.c
test_kw_if_else: $(PASS)/out.log.test_kw_if_else $(PASS)/err.log.test_kw_if_else
$(PASS)/out.log.test_kw_if_else $(PASS)/err.log.test_kw_if_else: src/kw_if_else.c $(TGT) 
	@echo Test $(STEP): Token/Statement Count: If/Else
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
