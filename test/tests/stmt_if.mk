all: test_stmt_if

.PHONY: test_stmt_if
test_stmt_if: STEP=test_stmt_if
test_stmt_if: $(PASS)/out.log.test_stmt_if $(PASS)/err.log.test_stmt_if
$(PASS)/out.log.test_stmt_if $(PASS)/err.log.test_stmt_if: src/stmt_if.c $(TGT) 
	@echo Test $(STEP): Statement Count: If
	$(TGT_E) $(EXCLUDE) src/stmt_if.c  --output-metrics=STMT_IF_CNT,TOK_IF -- $(OUT)
	$(CHECK_E)

