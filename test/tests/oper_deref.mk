all: test_oper_deref

.PHONY: test_oper_deref
test_oper_deref: STEP=test_oper_deref
test_oper_deref: $(PASS)/out.log.test_oper_deref $(PASS)/err.log.test_oper_deref
$(PASS)/out.log.test_oper_deref $(PASS)/err.log.test_oper_deref: src/oper_deref.c $(TGT) 
	@echo Test $(STEP): Operator Count: Pointer dereference
	$(TGT_E) $(EXCLUDE) src/oper_deref.c  --output-metrics=OP_DEREF* -- $(OUT)
	$(CHECK_E)

