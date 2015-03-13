all: test_param

.PHONY: test_param
test_param: STEP=test_param
test_param: METRICS=STMT_HIS_PARAM
test_param: FILES=src/param.c src/param2.c
test_param: $(PASS)/out.log.test_param $(PASS)/err.log.test_param
$(PASS)/out.log.test_param $(PASS)/err.log.test_param: src/param.c src/param2.c $(TGT)
	@echo Test $(STEP): Parameter Count
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
