# CMake generated Testfile for 
# Source directory: /mnt/d/syncwork/codespace/DTN/bondmscp
# Build directory: /mnt/d/SyncWork/CodeSpace/DTN/bondmscp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pytest "python3" "-m" "pytest" "-v" "--mscp-path=/mnt/d/SyncWork/CodeSpace/DTN/bondmscp/mscp" "/mnt/d/syncwork/codespace/DTN/bondmscp/test")
set_tests_properties(pytest PROPERTIES  WORKING_DIRECTORY "/mnt/d/SyncWork/CodeSpace/DTN/bondmscp" _BACKTRACE_TRIPLES "/mnt/d/syncwork/codespace/DTN/bondmscp/CMakeLists.txt;164;add_test;/mnt/d/syncwork/codespace/DTN/bondmscp/CMakeLists.txt;0;")
subdirs("libssh")
