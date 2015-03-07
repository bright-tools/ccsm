all: test_param

.PHONY: test_param
test_param: STEP=test_param
test_param: $(PASS)/out.log.test_param $(PASS)/err.log.test_param
$(PASS)/out.log.test_param $(PASS)/err.log.test_param: src/param.c src/param2.c $(TGT) 
	@echo Test $(STEP): Parameter Count
	$(TGT_E) $(EXCLUDE) src/param.c src/param2.c --output-metrics=STMT_HIS_PARAM -- $(OUT)
	$(CHECK_E)
