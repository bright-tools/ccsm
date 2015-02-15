all: test_single_file_excluded

.PHONY: test_single_file_excluded
test_single_file_excluded: STEP=test_single_file_excluded
test_single_file_excluded: $(PASS)/out.log.test_single_file_excluded $(PASS)/err.log.test_single_file_excluded
$(PASS)/out.log.test_single_file_excluded $(PASS)/err.log.test_single_file_excluded: src/empty.c $(TGT) 
	@echo Test $(STEP): Test of FILE_CNT \(single file excluded\)
	$(TGT_E) $(EXCLUDE) src/empty.c --exclude-file empty.c --output-metrics=FILE_CNT -- $(OUT)
	$(CHECK_E)

