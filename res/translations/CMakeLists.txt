# Exclude this directory from "make clean"
# Since the .ts files are technically build artifacts
# they get included in the clean rules
# Discussion: https://stackoverflow.com/questions/37960897/a-proper-workflow-for-cmake-qt-translation-system
set_directory_properties (PROPERTIES CLEAN_NO_CUSTOM 1)
