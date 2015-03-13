all: test_single_file_excluded

.PHONY: test_single_file_excluded
test_single_file_excluded: STEP=test_single_file_excluded
test_single_file_excluded: METRICS=FILE_CNT
test_single_file_excluded: FILES=src/empty.c
test_single_file_excluded: $(PASS)/out.log.test_single_file_excluded $(PASS)/err.log.test_single_file_excluded
$(PASS)/out.log.test_single_file_excluded $(PASS)/err.log.test_single_file_excluded: src/empty.c $(TGT)
	@echo Test $(STEP): Test of FILE_CNT \(single and only file excluded\)
	$(TGT_E) $(FILES) $(EXCLUDE) --exclude-file empty.c --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

