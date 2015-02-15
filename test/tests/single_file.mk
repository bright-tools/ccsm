all: test_single_file

.PHONY: test_single_file
test_single_file: STEP=test_single_file
test_single_file: $(PASS)/out.log.test_single_file $(PASS)/err.log.test_single_file
$(PASS)/out.log.test_single_file $(PASS)/err.log.test_single_file: src/empty.c $(TGT) 
	@echo Test $(STEP): Test of FILE_CNT \(single file\)
	$(TGT_E) $(EXCLUDE) src/empty.c --output-metrics=FILE_CNT -- $(OUT)
	$(CHECK_E)

