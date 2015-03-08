all: test_def_file

.PHONY: test_def_file
test_def_file: STEP=test_def_file
test_def_file: $(PASS)/out.log.test_def_file $(PASS)/err.log.test_def_file
$(PASS)/out.log.test_def_file $(PASS)/err.log.test_def_file: src/def_file.c src/def_file2.c $(TGT) 
	@echo Test $(STEP): Def File
	$(TGT_E) $(EXCLUDE) src/def_file.c src/def_file2.c --def-file=def_file.def --output-format=sparsetree -- $(OUT)
	$(CHECK_E)
