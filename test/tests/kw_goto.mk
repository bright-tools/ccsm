all: test_kw_goto

.PHONY: test_kw_goto
test_kw_goto: STEP=test_kw_goto
test_kw_goto: $(PASS)/out.log.test_kw_goto $(PASS)/err.log.test_kw_goto
$(PASS)/out.log.test_kw_goto $(PASS)/err.log.test_kw_goto: src/kw_goto.c src/kw_goto2.c $(TGT) 
	@echo Test $(STEP): Token/Statement Count: Goto
	$(TGT_E) $(EXCLUDE) src/kw_goto.c src/kw_goto2.c  --output-metrics=STMT_GOTO_CNT,TOK_GOTO -- $(OUT)
	$(CHECK_E)
