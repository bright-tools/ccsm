all: test_single_file_multiple

.PHONY: test_single_file_multiple
test_single_file_multiple: STEP=test_single_file_multiple
test_single_file_multiple: METRICS=FILE_CNT
test_single_file_multiple: FILES=src/empty.c src/empty.c
test_single_file_multiple: $(PASS)/out.log.test_single_file_multiple $(PASS)/err.log.test_single_file_multiple
$(PASS)/out.log.test_single_file_multiple $(PASS)/err.log.test_single_file_multiple: src/empty.c $(TGT)
	@echo Test $(STEP): Test of FILE_CNT \(same file\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

