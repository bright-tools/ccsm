all: test_single_file

.PHONY: test_single_file
test_single_file: STEP=test_single_file
test_single_file: METRICS=FILE_CNT
test_single_file: FILES=src/empty.c
test_single_file: $(PASS)/out.log.test_single_file $(PASS)/err.log.test_single_file
$(PASS)/out.log.test_single_file $(PASS)/err.log.test_single_file: src/empty.c $(TGT)
	@echo Test $(STEP): Test of FILE_CNT \(single file\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

