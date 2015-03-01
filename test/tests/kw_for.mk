all: test_kw_for

.PHONY: test_kw_for
test_kw_for: STEP=test_kw_for
test_kw_for: $(PASS)/out.log.test_kw_for $(PASS)/err.log.test_kw_for
$(PASS)/out.log.test_kw_for $(PASS)/err.log.test_kw_for: src/kw_for.c src/kw_for2.c $(TGT) 
	@echo Test $(STEP): Token/Statement Count: For
	$(TGT_E) $(EXCLUDE) src/kw_for.c src/kw_for2.c  --output-metrics=STMT_FOR_CNT,TOK_FOR -- $(OUT)
	$(CHECK_E)
