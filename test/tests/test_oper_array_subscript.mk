all: test_oper_array_subscript

.PHONY: test_oper_array_subscript
test_oper_array_subscript: STEP=test_oper_array_subscript
test_oper_array_subscript: METRICS=OP_ARRAY_SUBSCRIPT_CNT
test_oper_array_subscript: FILES=src/oper_array_subscript.c
test_oper_array_subscript: $(PASS)/out.log.test_oper_array_subscript $(PASS)/err.log.test_oper_array_subscript
$(PASS)/out.log.test_oper_array_subscript $(PASS)/err.log.test_oper_array_subscript: src/oper_array_subscript.c $(TGT)
	@echo Test $(STEP): Operator Count: Array Subscript
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

