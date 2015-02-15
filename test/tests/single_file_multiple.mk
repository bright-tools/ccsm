all: test_single_file_multiple

.PHONY: test_single_file_multiple
test_single_file_multiple: STEP=test_single_file_multiple
test_single_file_multiple: $(PASS)/out.log.test_single_file_multiple $(PASS)/err.log.test_single_file_multiple
$(PASS)/out.log.test_single_file_multiple $(PASS)/err.log.test_single_file_multiple: src/empty.c $(TGT) 
	@echo Test $(STEP): Test of FILE_CNT \(same file\)
	$(TGT_E) $(EXCLUDE) src/empty.c src/empty.c --output-metrics=FILE_CNT -- $(OUT)
	$(CHECK_E)

