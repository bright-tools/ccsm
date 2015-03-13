all: test_two_files

.PHONY: test_two_files
test_two_files: STEP=test_two_files
test_two_files: METRICS=FILE_CNT
test_two_files: FILES=src/empty.c src/comment.c 
test_two_files: $(PASS)/out.log.test_two_files $(PASS)/err.log.test_two_files
$(PASS)/out.log.test_two_files $(PASS)/err.log.test_two_files: src/comment.c src/empty.c $(TGT) 
	@echo Test $(STEP): Test of FILE_CNT \(two source files and include\)
	$(TGT_E) $(FILES) $(EXCLUDE) --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)

