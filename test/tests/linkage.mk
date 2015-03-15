all: test_linkage

.PHONY: test_linkage
test_linkage: STEP=test_linkage
test_linkage: METRICS=OP_FN_CALL_CNT,OP_FN_CALL_UNIQUE_CNT,LOCAL_FN_CALL_CNT,FUNC_CALLED_BY
test_linkage: FILES=src/linkage.c src/linkage2.c
test_linkage: $(PASS)/out.log.test_linkage $(PASS)/err.log.test_linkage
$(PASS)/out.log.test_linkage $(PASS)/err.log.test_linkage: src/linkage.c src/linkage2.c $(TGT)
	@echo Test $(STEP): Linkage
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
