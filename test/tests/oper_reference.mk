all: test_oper_reference

.PHONY: test_oper_reference
test_oper_reference: STEP=test_oper_reference
test_oper_reference: $(PASS)/out.log.test_oper_reference $(PASS)/err.log.test_oper_reference
$(PASS)/out.log.test_oper_reference $(PASS)/err.log.test_oper_reference: src/oper_reference.c $(TGT) 
	@echo Test $(STEP): Operator Count: Rereference - "address of"
	$(TGT_E) $(EXCLUDE) src/oper_reference.c  --output-metrics=OP_ADDR_OF_CNT -- $(OUT)
	$(CHECK_E)

